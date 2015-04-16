/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\scr\ThDySec\sec_mult.cpp
*
* @brief 
*/

#ifdef WINDOWS_FORM_GUI
#include "stdafx.h"
#pragma unmanaged
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <math.h>
#include <list>
#include <stack>

using namespace std ; 

#include "ThDySec/sec_mult.h"
#include "ThDySec/common.h" 
using namespace DegCod;


CMultSec::CMultSec (	const std::string &file	, 
					std::shared_ptr<CSaltCorrNN>  NNpar	, 
					bool           all_dir  /*= false*/,
					float		   MaxTgId	/*= 100*/, 
					LonSecPosRang  SecLim	/*= LonSecPosRang {1,0}*/,	 
                    SecPosRang     SecLenLim/*= SecPosRang    {0,0}*/,
					bool           loadSec  /*= true*/
				 )  
    :	/*_name(trim_string(file)),	*/
	    _SecLim     (SecLim),
	    _MaxTgId    (MaxTgId), 
        _SecLenLim  (SecLenLim),
	    _NNPar      (NNpar)/*,
        _Path       (file)*/
{
	filesystem::path  itf(file);

	if (all_dir)
    {
        if (filesystem::is_regular_file(itf)) itf.remove_filename();

	    _name = itf.filename ();     /// The new MSec take the name of the dir.
        _Path = itf;                 /// and the _Path point to it.

        filesystem::directory_iterator rdi{ itf }, end;

        for (; rdi != end; ++ rdi)
            AddMultiSec(  new CMultSec(  rdi->path().string().c_str() , 
                                         NNpar,
                                         filesystem::is_directory(rdi->status()), 
                                         MaxTgId, 
                                         SecLim,   
                                         SecLenLim,
                                         loadSec) );
          //      else 
          //          itf.remove_filename();
          //      _name = itf.   .basename();
		        //AddFromDir(file);
	}
	else
	    if (itf.has_filename())
	    {
		    _name = itf.filename ();     /// The new MSec take the name of the file.
            _Path = itf;                 /// and the _Path point directly to the file.
            if (loadSec)
		       AddFromFile(file);  /// will throw if not a file
	    }
}


//int		CMultSec::AddFromDir(const std::string& dir, bool  recurs  /*= false*/)		// return la cantidad de sec add --------------------  AddFromDir   -------------------
//{	
//	filesys::path  itf(dir);
//
//	if (recurs)
//    {
//        filesys::directory_iterator rdi{filesys::is_directory(itf) ? itf : itf.remove_filename() }, end;
//	    _name = itf.filename ();
//
//        for (; rdi != end; ++ rdi)
//            AddMultiSec(  new CMultSec(  rdi->path().string().c_str() , 
//                                         _NNpar,
//                                         filesys::is_directory(rdi->status()), 
//                                         _MaxTgId, 
//                                         _SecLim,   
//                                         _SecLenLim  ) );
//  //      else 
//  //          itf.remove_filename();
//  //      _name = itf.   .basename();
//		//AddFromDir(file);
//	}
//	else
//	    if (itf.has_filename())
//	    {
//		    _name = itf.filename ();
//		    AddFromFile(file);
//	    }
//}
int		CMultSec::AddFromFile (const std::string& file)		// return la cantidad de sec add --------------------  AddFromFile   -------------------
{	
    ifstream ifile( file ); 
	if ( ! ifile ) 
	{
	    throw std::ios_base::failure(string("Could not open the sequence file: ")+ file );
	}

	return AddFromFile(ifile); /// \todo: retrow anadiendo el nombre del file
}

int		CMultSec::AddFromFile (ifstream& ifile)		// return la cantidad de sec add --------------------  AddFromFile   -------------------
{		
	if (  _SecLim.Max() <= _SecLim.Min() ) 
		_SecLim.SetMax(0) ; // if ( _SecEnd<=_SecBeg) _SecEnd=0 ;

	int j=0;
	char c1;
	ifile>>skipws  >> c1;
	if ( ! ifile.good() ) 	
	{
	    throw std::ios_base::failure(string("Could not read the sequence file: ")/*+ file */);
	}

	if( c1 =='>' ) 																		
		return AddFromFileFASTA (ifile);
					// esto es FASTA, si no  BLAST o GB o ...
	if (c1 =='<' )
	{	string xml_DOCTYPE ;
		getline (ifile, xml_DOCTYPE,'>') ;   // <?xml version="1.0"?>
		getline (ifile, xml_DOCTYPE,'>') ;   // <!DOCTYPE BlastOutput PUBLIC "-//NCBI//NCBI BlastOutput/EN" "NCBI_BlastOutput.dtd">
		if ( ! ifile.good() ) 	
		{	throw std::ios_base::failure(string("Could not open the sequence file: ")/*+ file*/ );		}

		if		(string::npos!=xml_DOCTYPE.find("DOCTYPE BlastOutput") )				
			return AddFromFileBLAST (ifile);
		else if (string::npos!=xml_DOCTYPE.find("DOCTYPE Bioseq-set" ) )				
			return AddFromFileGB (ifile);
		return 0;
	}
	if (c1 =='L' )	
	{	ifile.putback(c1);												
		return AddFromFileGBtxt (ifile);	
	}
	return 0;   // cerr unknow format		
}	 

int		CMultSec::AddFromFileFASTA (ifstream &ifile)  // -------------------    AddFromFileFASTA   ------------
{	int j=0 ;													//long l= (_SecEnd ? _SecEnd-_SecBeg +1 : 0) ;
	string Descriptor  ;
	while (getline (ifile, Descriptor) )
    {
		size_t b_d=Descriptor.find_first_not_of(
								"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890!#$()*+-./<=>@[]^_{|}~"    );
		string Fasta_NAME = Descriptor.substr(0,b_d );
		Descriptor=Descriptor.substr(Fasta_NAME.length());

  		string Fasta_SEC ;					
		if (!getline (ifile, Fasta_SEC,'>')) 		break ;		

		if ( Fasta_SEC.length() < _SecLim.Min() ||  Fasta_SEC.length() < _SecLenLim.Min() )	continue;

		unique_ptr<CSec> sec (  new CSec(   Fasta_SEC , 
                                            _Local._NSec, 
                                            Fasta_NAME , 
                                            _NNPar,
                                            _SecLim.Max() ? _SecLim.Max()-_SecLim.Min()+1 : 0 ,
                                            _SecLim.Min()                                       /// \todo: cambiar constr de CSec ????
                                         )) ;                  assert(sec);
						
		if ( sec->Len() >= _SecLenLim.Min()   )		
		{	
			CSec *idem=Idem(*sec);
			if (idem) 
			{
				sec->Selected(false);
				sec->Filtered(true);
			}
			else
				j++;		
			sec->Description(trim_string(Descriptor));
			InsertSecAfter ( sec.release() , idem) ;	
		}
	}
	return j;	
}

int		CMultSec::AddFromFileBLAST (ifstream &fi) // ----------------  CMultSec::            AddFromFileBLAST  -----------------------------
{	unsigned int	_BlastOutput_query_len ;		// x todos los "hits"
	int			 id=0;
	string li ;  //  xml_line

	do  {	getline (fi, li,'>') ;			if ( ! fi.good() ) return 0; }   // BLAST format error
	while  (string::npos==li.find("BlastOutput_query-len") ); fi>>_BlastOutput_query_len;//  <BlastOutput_query-len>267</BlastOutput_query-len>
	
	do {	unsigned int	_Hit_num=0 ;			// para cada hit
			std::string	    _Hit_id  ;				
			std::string	    _Hit_def ;				// descriptor ??
			std::string	    _Hit_accession 	;
			long			_Hit_len=0 ;				
			float			_Hsp_bit_score=0 ;
			unsigned int	_Hsp_score=0 ;
			double			_Hsp_evalue=0 ;
			LonSecPos		_Hsp_query_from=0 ;    // dejar signed or unsigned !!!!????
			LonSecPos		_Hsp_query_to=0 ;
			LonSecPos 		_Hsp_hit_from=0 ;
			LonSecPos 		_Hsp_hit_to=0 ;
			int				_Hsp_query_frame=0 ;
			int				_Hsp_hit_frame=0 ;
			LonSecPos 		_Hsp_identity=0 ; // revisar type --- no sera %  : float??
			LonSecPos 		_Hsp_positive=0 ;
			LonSecPos 		_Hsp_gaps=0 ;
			LonSecPos 		_Hsp_align_len=0 ;
			std::string	    _Hsp_midline ;
			bool			_FormatOK=0 ;
		std::string	   sec;						// para CSec
		std::string	   nam;
		long		   l=0;
		std::string	   clas;

	while(getline(fi,li,'>')&& string::npos==li.find("Hit_num"      ) ) ;  fi>>_Hit_num;				//  <Hit_num>1</Hit_num>
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_id"       ) ) ;                           	//<Hit_id>gi|84028434|gb|DQ318020.1|</Hit_id> 
                                                            if ( ! getline (fi, _Hit_id, '<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_def"      ) ) ;                               //<Hit_def>Wets NIle virus strain ArB3573/82, complete genome</Hit_def>
                                                            if ( ! getline (fi, _Hit_def,'<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_accession") ) ;                              	//<Hit_def>Wets NIle virus strain ArB3573/82, complete genome</Hit_def>
                                                            if ( ! getline (fi, _Hit_accession,'<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hit_len"      ) ) ;  fi>>_Hit_len;				//  <Hit_len>11048</Hit_len> 
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_bit-score") ) ;  fi>>_Hsp_bit_score;			//  <Hsp_bit-score>482.786</Hsp_bit-score>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_score"    ) ) ;  fi>>_Hsp_score;              //  <Hsp_score>534</Hsp_score>		
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_evalue"   ) ) ;  fi>>_Hsp_evalue;		    	//  <Hsp_evalue>3.71782e-133</Hsp_evalue>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_query-from")) ;  fi>>_Hsp_query_from;		    //  <Hsp_query-from>1</Hsp_query-from>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_query-to" ) ) ;  fi>>_Hsp_query_to;			//  <Hsp_query-to>267</Hsp_query-to>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_hit-from" ) ) ;  fi>>_Hsp_hit_from;			//  <Hsp_hit-from>9043</Hsp_hit-from>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_hit-to"   ) ) ;  fi>>_Hsp_hit_to;			    //  <Hsp_hit-to>9309</Hsp_hit-to>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_query-frame"));  fi>>_Hsp_query_frame;		//  <Hsp_query-frame>1</Hsp_query-frame>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_identity" ) ) ;  fi>>_Hsp_identity;			//  <Hsp_identity>267</Hsp_identity>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_positive" ) ) ;  fi>>_Hsp_positive;			//  <Hsp_positive>267</Hsp_positive>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_gaps"     ) ) ;  fi>>_Hsp_gaps;	 		    //  <Hsp_gaps>0</Hsp_gaps>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_align-len") ) ;  fi>>_Hsp_align_len;		    //  <Hsp_align-len>267</Hsp_align-len>
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_hseq") ) ; 	                            	//  <Hsp_hseq>TACAACATGATGGGAAAGAGAGAGAAGAAG 
                                                            if ( ! getline (fi, sec         ,'<') ) return id;
	while(getline(fi,li,'>')&& string::npos==li.find("Hsp_midline") ) ;                                 //  <Hsp_midline>|||||||||||||||||||||||||||||||||||||||||
                                                            if ( ! getline (fi, _Hsp_midline,'<') ) return id;

			// long SecBeg = _SecBeg - _Hsp_query_from +1 ;			// if (_SecBeg >= _Hsp_query_from)
			//if ( (_SecBeg		<= _Hsp_query_to) && ( (!_SecEnd)		 || _SecEnd		  >=_Hsp_query_from) ) // _SecEnd=0 significa no recortar la sec.

			if ( (_SecLim.Min() <= _Hsp_query_to) && ( (!_SecLim.Max())  || _SecLim.Max() >=_Hsp_query_from) ) // _SecLim.Max()=0 significa no recortar la sec.
			{	
                std::unique_ptr<CSecBLASTHit> secH
                    { new CSecBLASTHit(      _BlastOutput_query_len ,
											// para cada hit
											_Hit_num ,
											std::move(_Hit_id) ,				
											std::move(_Hit_def) ,				
											std::move(_Hit_accession)	,
											_Hit_len ,				
											_Hsp_bit_score ,
											_Hsp_score ,
											_Hsp_evalue ,
											_Hsp_query_from ,
											_Hsp_query_to ,
											_Hsp_hit_from ,
											_Hsp_hit_to ,
											_Hsp_query_frame ,
											_Hsp_hit_frame ,
											_Hsp_identity ,
											_Hsp_positive ,
											_Hsp_gaps ,
											_Hsp_align_len ,
											std::move(_Hsp_midline) ,
											_FormatOK ,
											std::move(sec)		,_SecLim,                         // _SecBeg, _SecEnd,
											id,			                    //Hit_num   ???		//	char	*	nam,	Hit_def
											_NNPar               /*,  //long l=0,(Hit_len ---> NO ) !!!  -->_Hsp_align_len -OK clas,	conc*/
											)
                    };

				if ( secH->Len() >= _SecLenLim.Min()  )		
				{	
					CSec *idem=Idem(*secH);
					InsertSecAfter (secH.release()  , idem) ;	
					if (idem) 
					{
						secH->Selected(false);
						secH->Filtered(true);
					}
					id++;		
				}
			}
		}
	while (fi.good() ); 
	return id; 
}

int		CMultSec::AddFromFileGBtxt (ifstream &ifile) // ----------------  CMultSec::            AddFromFileGBtxt  -----------------------------
{	const int gb_descr_w=12 ;	char gb_descr[gb_descr_w+1]; gb_descr[gb_descr_w]=0;
	size_t strl;
	int	id	=0     ;
	string txt_line ;

	do {	char	*	LOCUS			=nullptr      ;
			long		Seq_inst_length	=0		;	
			char	*	DEFINITION		=nullptr     ;
			char	*	ACCESSION		=nullptr     ;
			char	*	ORGANISM		=nullptr     ;
			// para CSec
			char	*	sec		=nullptr     ;		//	char		*	nam,	DEFINITION	,	//	long			l=0,		Seq_inst_length

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// LOCUS       AY702040               10675 bp    RNA     linear   VRL 24-MAR-2005
			while  (strcmp(gb_descr,	"LOCUS"	) );
			ifile>>setw (gb_descr_w)>>gb_descr ;				LOCUS=new char[1+(strl=strlen(gb_descr))] ;
			strncpy(LOCUS,gb_descr,strl) ;						LOCUS	[strl]=0;
			ifile>>Seq_inst_length ;	

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// DEFINITION  Dengue virus type 2 strain I348600, complete genome.
			while  (strcmp(gb_descr,	"DEFINITION"	) );
			getline (ifile, txt_line) ;								DEFINITION	=new char[txt_line.length()+1] ;
			txt_line.copy(  DEFINITION  ,txt_line.length()) ;		DEFINITION	[txt_line.length()]=0;

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// ACCESSION   AY702040
			while  (strcmp(gb_descr,	"ACCESSION"	) );
			ifile>>setw (gb_descr_w)>>gb_descr ;			ACCESSION=new char[1+(strl=strlen(gb_descr))] ;
			strncpy(ACCESSION,gb_descr,strl) ;						ACCESSION	[strl]=0;

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	//   ORGANISM  Dengue virus 2
			while  (strcmp(gb_descr,	"ORGANISM"	) );
			getline (ifile, txt_line) ;							ORGANISM	=new char[txt_line.length()+1] ;
			txt_line.copy(  ORGANISM  ,txt_line.length()) ;		ORGANISM	[txt_line.length()]=0;

			do  {	ifile>>setw (gb_descr_w)>>gb_descr ;	if ( ! ifile.good() ) return id; } 	// ORIGIN      
			while  (strcmp(gb_descr,	"ORIGIN"	) );
			getline (ifile, txt_line,'/') ;							sec	=new char[txt_line.length()+1] ;
			txt_line.copy(  sec  ,txt_line.length()) ;				sec	[txt_line.length()]=0;

			CSecGBtxt *secGBtxt= new CSecGBtxt(	LOCUS       ,
												Seq_inst_length,	
												DEFINITION     ,
												ACCESSION      ,
												ORGANISM       ,
												sec	,	
												id,								//	char		*	nam,	DEFINITION	,	
												_NNPar);
				if ( secGBtxt->Len() >= _SecLenLim.Min()   )		
				{	
					CSec *idem=Idem(*secGBtxt);
					InsertSecAfter (secGBtxt  , idem) ;	
					if (idem) 
					{
						secGBtxt->Selected(false);
						secGBtxt->Filtered(true);
					}
					else
						id++;		
				}
				else delete secGBtxt;
			delete []sec ;
		}
	while (ifile.good() ); 
	return id; 
}

int		CMultSec::AddFromFileGB (ifstream &ifile)  // ----------------  CMultSec::            AddFromFileGB  -----------------------------
{	int		id=0;
	string xml_line ;

	do {	char		*	_Textseq_id_accession	=0 ;	
			char		*	_Org_ref_taxname		=0 ;
			char		*	_Seqdesc_title			=0 ;
			long			_Seq_inst_length		=0 ;	
			// para CSec
			char		*sec=0;			//char		*nam=0;		//long		 l=0;		//char		*clas=0;

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; } 	// <Textseq-id_accession>DQ318020</Textseq-id_accession>
			while  (string::npos==xml_line.find(	"Textseq-id_accession"	) );
			getline (ifile, xml_line,'<') ;				_Textseq_id_accession=new char[xml_line.length()+1] ;
			xml_line.copy(_Textseq_id_accession,xml_line.length()) ;	_Textseq_id_accession	[xml_line.length()]=0;

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; } // <Org-ref_taxname>West Nile virus</Org-ref_taxname>
			while  (string::npos==xml_line.find(	"Org-ref_taxname"	) );
			getline (ifile, xml_line,'<') ;								_Org_ref_taxname	=new char[xml_line.length()+1] ;
			xml_line.copy(  _Org_ref_taxname  ,xml_line.length()) ;		_Org_ref_taxname	[xml_line.length()]=0;
		// <Dbtag_db>taxon</Dbtag_db>
		// <Object-id_id>11082</Object-id_id>
		// <OrgName_name_virus>West Nile virus</OrgName_name_virus>
		// <OrgMod_subtype value="strain">2</OrgMod_subtype>
		// <OrgMod_subname>ArB3573/82</OrgMod_subname>
		// <OrgMod_subtype value="gb-acronym">32</OrgMod_subtype>
		// <OrgMod_subname>WNV</OrgMod_subname>
		// <OrgName_lineage>Viruses; ssRNA positive-strand viruses, no DNA stage; Flaviviridae; Flavivirus; Japanese encephalitis virus group</OrgName_lineage>
		// <SubSource_subtype value="country">23</SubSource_subtype>
		// <SubSource_name>Central African Republic</SubSource_name>
		// <SubSource_subtype value="other">255</SubSource_subtype>
		// <SubSource_name>lineage 2; SMB pass 4, C6/36 pass 1</SubSource_name>
			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }		// <Seqdesc_title>Wets NIle virus strain ArB3573/82, complete genome</Seqdesc_title>
			while  (string::npos==xml_line.find(	"Seqdesc_title"	) );
			getline (ifile, xml_line,'<') ;								_Seqdesc_title	=new char[xml_line.length()+1] ;
			xml_line.copy(  _Seqdesc_title  ,xml_line.length()) ;		_Seqdesc_title	[xml_line.length()]=0;
		// <MolInfo_biomol value="mRNA">3</MolInfo_biomol>
		// <MolInfo_completeness value="complete">1</MolInfo_completeness>
		// <Date-std_year>2006</Date-std_year>
		// <Date-std_month>1</Date-std_month>
		// <Date-std_day>1</Date-std_day>
		// <Seq-inst_mol value="rna"/>

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }  // GB format error
			while  (string::npos==xml_line.find("Seq-inst_length") ) ; ifile>>_Seq_inst_length;		// <Seq-inst_length>11048</Seq-inst_length>
			
		// <Seq-inst_strand value="ss"/>

			do  {	getline (ifile, xml_line,'>') ;	if ( ! ifile.good() ) return id; }	// <IUPACna>AGTAGTTCGCCTGTGTGAGCTGACA.... GGTGCTAGAACACAGGATCT</IUPACna>
			while  (string::npos==xml_line.find(  "IUPACna"  ) );
			getline (ifile, xml_line,'<') ;         sec=new char[xml_line.length()+1] ;
			xml_line.copy(sec,xml_line.length()) ;	sec[xml_line.length()]=0;	


			CSecGB *secGB=  new CSecGB(      _Textseq_id_accession,
											_Org_ref_taxname	,
											_Seqdesc_title,
											_Seq_inst_length	 ,
											sec	,	
											id,			//	char		*	nam,		Hit_def
											_NNPar/*,  	//	long			l=0,		(Hit_len ---> NO ) !!!  -->_Hsp_align_len -OK clas, conc*/
										);
			delete []sec ;

				if ( secGB->Len() >= _SecLenLim.Min()  )		
				{	
					CSec *idem=Idem(*secGB);
					InsertSecAfter (secGB  , idem) ;	
					if (idem) 
					{
						secGB->Selected(false);
						secGB->Filtered(true);
					}
					else
						id++;		
				}
				else delete secGB;
		}
	while (ifile.good() ); 
	return id; 
}
	
int		CMultSec::AddFromFileODT (ifstream &ifileODT){return 0;}
int		CMultSec::AddFromFileODS (ifstream &ifileODS){return 0;}

CSec	*CMultSec::Idem ( CSec &sec )   // ----------------  CMultSec::            NotIdem  --- busqueda trivial de sec identicas -------------
{	if ( _MaxTgId >= 100  ) return nullptr ;
	long Lcs=sec.Len() ;											// len of candidate sec (to be in the list, with MaxId)
	long MaxErCS= long(ceil(float(Lcs*(100.0f-_MaxTgId) ) / 100.0f));					// min of not Id base to be in the list
	for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las primeras sec de esta misma ms
	{	CSec &s = *CurSec() ; 
		if (s.Filtered()) continue;
		long l, MaxEr ;
		if (s.Len() < Lcs) 		{	l=s.Len() ;			MaxEr= long(ceil(l*(100-_MaxTgId)  / 100));		} 
		else 					{	l=Lcs ;				MaxEr= MaxErCS;						}
		long i=0 , Er=0 ;
		while ( ++i <=l ) 	if ( MaxEr   <   ( Er += ( s[i] != sec[i] )) )  break;
		if (i>l) return &s ;		
	}
	return nullptr ;
}
 CSec *	CMultSec::AddSec ( CSec *sec )
{	if (!sec) return nullptr ;
	_LSec.Add(sec);
	UpdateTotalsAdding ( sec );
	return sec;
}
 CSec *	CMultSec::InsertSec ( CSec *sec ) // insert 'link' sec ,  antes de 'Cur'
{	if (!sec) return nullptr ;
	_LSec.Insert(sec);
	UpdateTotalsAdding ( sec );
	return sec;
}
 CSec *	CMultSec::InsertSecAfter	( CSec *sec , CSec *preSec )			// insert 'link' sec ,  after preSec
{	
	if (!preSec) return AddSec (sec);
	if (!sec) return nullptr ;
	sec->InsertAfter( preSec);
	UpdateTotalsAdding ( sec );
	return sec;
}
void	CMultSec::UpdateTotalsAdding ( CSec *sec ) 
{	
	if (!sec || sec->_parentMS == this)										// no hay sec o ya estaba aqui
		return;
	CMultSec *parMS   =sec->_parentMS;					// /*._Get()*/
	CMultSec *My_parMS=     _parentMS;					// /*._Get()*/
	bool checkExtr(true) ; 
	CMultSec   *cp;

	if (parMS)
	{
	    cp=findComParent( parMS);
		parMS->_Local._NSec--	;								// la elimino de la ms orig. 
		//parMS->_Global._NSec -- ;								// 
		if (parMS->_Local._NSec)
			if (parMS->isLocExtreme(sec))
				RecalExtremes();
			else
				checkExtr=false;
		else
			checkExtr=false;

		for ( /*parMS=parMS->*/_parentMS;  parMS != cp &&  parMS ;  parMS=parMS->_parentMS)		// desde localizacion orig subiendo hasta parent comun
			{
				parMS->_Global._NSec -- ;								// elimino  de este total.
				if (checkExtr && parMS->_Global._NSec)
					if (parMS->isGlobExtreme(sec))
						RecalExtremes();
					else
						checkExtr=false;
				else
					checkExtr=false;
			}
	}else
		cp=nullptr;
	Add2LocalExtreme(*sec);
	for (My_parMS ; My_parMS!=cp && My_parMS; My_parMS=My_parMS->_parentMS)  // desde mi hacia arriba hasta el com parent anadiendo
	{
		if (checkExtr)
			My_parMS->Add2GlobalExtreme(*sec);
		else
		{
			parMS->_Global._NSec ++;								// sumo sus s a este total.
		}
	}	
	sec->_parentMS = (this) ;							//* std::weak_ptr<CMultSec> */
}


CMultSec   *CMultSec::findComParent( CMultSec *ms)
{
	if(!ms || ms==this) 
		return ms;
	std::stack<CMultSec*> myTree,		oTree;
	CMultSec			*myPms=this, *oPms=ms;
		myTree.push(myPms);
		oTree.push(oPms);

	do 
	{	myPms=myPms->_parentMS;
		myTree.push(myPms);
	}while (myPms);

	do 
	{	oPms=oPms->_parentMS;
		oTree.push(oPms);
	}while (oPms);

	do 
	{	if(  (oPms = myTree.top())   !=  oTree.top()  )
			return myPms;
		myPms= oPms;
		myTree.pop();
		oTree.pop();
	} while (!myTree.empty() && !oTree.empty() );
	return myPms;
}


CMultSec *	CMultSec::AddMultiSec ( CMultSec *ms )  //--------------------------------------    AddMultiSec    --------------------
{	if (!ms) return nullptr;	
	_LMSec.Add(ms);
	UpdateTotalsAdding ( ms );   // al llamar ya esta la ms movida fisicamente. Falta solo actualizar extremes
	return ms;
}
void	    CMultSec::UpdateTotalsAdding ( CMultSec *msec ) 
{	
	if (!msec || msec->_parentMS==this)					// no hay msec o ya estaba antes en una de mis subtrees inmediatas. 
		return;

	CMultSec *parMS   =msec->_parentMS;					// /*._Get()*/
	CMultSec *My_parMS=     _parentMS;					// /*._Get()*/
	bool checkExtr(true) ; 
	CMultSec   *cp;
	if (parMS)										// no es imprescindible. Anadido solo por claridad de intencion
	{	cp=findComParent( msec);
		for ( parMS;  parMS != cp   ;  parMS=parMS->_parentMS)			// desde localizacion orig subiendo hasta parent comun
			{
				parMS->_Global._NSec -= msec->_Global._NSec ;			// elimino sus s de este total.
				parMS->_Global._NMSec-= msec->_Global._NMSec + 1;		// elimino sus ms de este total.
				parMS->_Local._NMSec--	;								// la elimino de esta ms. 
				if (checkExtr && parMS->_Global._NSec)
					if (parMS->isGlobExtreme(msec))
						RecalExtremes();
					else
						checkExtr=false;
			}
	}else
		cp=nullptr;
	Add2LocalExtreme(*msec);
	for (My_parMS ; My_parMS!=cp && My_parMS; My_parMS=My_parMS->_parentMS)  // desde mi hacia arriba hasta el com parent anadiendo
	{
		if (checkExtr)
			My_parMS->Add2GlobalExtreme(*msec);
		else
		{
			My_parMS->_Global._NSec += msec->_Global._NSec ;			// sumo sus s a este total.
			My_parMS->_Global._NMSec+= msec->_Global._NMSec + 1;		// sumo sus ms a este total.
		}
	}
	msec->_parentMS = (this) ;							// std::weak_ptr<CMultSec> 

}

		CMultSec::~CMultSec ()				// funciona bien solo si la lista es "lineal"
{	_LSec.Destroy  ()	;
	_LMSec.Destroy ()	;
	Remove(); //? in ~CLink !
	// CSec		*_Consenso ;
}    


//void	CMultSec::RefreshExtremes( CMultSec *ms)
//{
//	if(!ms)
//		return;
//	if(ms->_NSec)
//
//}
	//	
	//while (parMS)									// subo por el tree hasta llegar al root
	//	if (parMS==this)						
	//		return;									// la sec estaba antes en una de mis subtrees. 
	//	else
	//auto TLen= msec->_TLen ;
	//auto TTm = msec->_TTm ;
	////auto TNMS
	//CMultSec *My_parMS=this ;    //_parentMS/*._Get()*/;		// 
	//while (My_parMS)								// subo por el tree hasta llegar al root
	//{	if ( ! My_parMS->_TNSec  )	
	//	{											
	//		My_parMS->_TLen= TLen ;				//   si ademas es la primera del todo inicializar los max totales
	//		My_parMS->_TTm = TTm ;
	//	}else
	//		{	
	//			My_parMS->_TLen.Expand( TLen) ;
	//			My_parMS->_TTm .Expand( TTm ) ;
	//		}	
	//	My_parMS->_TNSec +=msec->_TNSec;			// la elimino de este total. Que hacer con las max??
	//	My_parMS->_TNMSec+=msec->_TNMSec;
	//	TLen=My_parMS->_TLen ;
	//	TTm =My_parMS->_TTm ;
	//}