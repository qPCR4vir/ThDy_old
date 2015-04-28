/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDySec\sec_mult.h
*
* @brief 
*/

#pragma unmanaged	

#ifndef _MULTSEC_H
#define _MULTSEC_H

#include <stdlib.h>
#include <fstream>
#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>
namespace filesystem = std::tr2::sys; //std::experimental::filesystem


using namespace std;

#include "sec.h" 
#include "sec_rang.h" 

  /// @brief permite hacer grupos de sec o de MultiSec (para analisis por "especies"?)
  /// \todo anadir posibilidad de construir sec concenso 
  /// CUIDADO :  It owns the sequences y las borra en su destructor: Usar Remove() or Free() para evitarlo
class CMultSec	 : public CLink	// ---------------------------------------------------------- 	CMultSec    -------------------
{	public:
		std::string			_name ;						///< 
        int					_ID       {NewMS_ID()};		///< Unique ID in each programm run
        LonSecPosRang       _SecLim   {1,0};			///< \todo: quitar de aqui?. Pertenece a CSec, o a un objeto "AddFromFile" 
        SecPosRang          _SecLenLim{0,0};            ///< \todo: NumRang<LonSecPos> _SecRange{1,0};// def-unlimited: the range used, not the limits
		float				_MaxTgId  {100};			///< \todo: quitar de aqui?. Pertenece a CSec, o a un objeto "AddFromFile" 
		std::shared_ptr<CSaltCorrNN>	_NNPar ;		///< \todo: quitar de aqui?. Pertenece a CSec, o a un objeto "AddFromFile" 
		CMultSec			*_parentMS {nullptr};		///< std::weak_ptr<CMultSec> _parentMS	;
        CSec				*_Consenso {nullptr};
        bool                 _selected { true };
		std::string			 _Path ;				///< file path of the original sequence source


     //explicit CMultSec (const std::string &Name  )                 : _name		(trim_string(Name))  {	}

   explicit CMultSec (std::shared_ptr<CSaltCorrNN> NNpar, const std::string &Name = "")        
                      : _NNPar      (NNpar            ), 
					    _name       (trim_string(Name))            
                  {  }

         CMultSec(CMultSec	*ms, const std::string &Name = ""): _name       (trim_string(Name)),
                                                                _SecLim     (ms->_SecLim),
                                                                _SecLenLim  (ms->_SecLenLim),
                                                                _MaxTgId    (ms->_MaxTgId), 
                                                                _NNPar      (ms->_NNPar)         
                  {  }

		 CMultSec (	ifstream &	    file	,	 
					std::shared_ptr<CSaltCorrNN>  NNpar	, 
					float		  MaxTgId	= 100, 
					LonSecPosRang  SecLim	= LonSecPosRang {1,0}, 
                    SecPosRang     SecLenLim= SecPosRang{0,0})  /*_name(trim_string(file)),	*/
                  : 
	                    _SecLim     (SecLim),
                        _SecLenLim  (SecLenLim),
	                    _MaxTgId    (MaxTgId), 
	                    _NNPar      (NNpar)              
                  { AddFromFile(file); }

         /// The new MSec take the name of the dir, and remember the rest of the path
         CMultSec (	const std::string &path	,                     ///< The name of the file or directory to be loaded 
					std::shared_ptr<CSaltCorrNN>  NNpar	, 
					bool           all_dir  = false,              ///< Load all files and directories recursiverly? 
					float		   MaxTgId	= 100,                ///< Sec. with more % of idem are marked as "filtered" and not selected
					LonSecPosRang  SecLim	= LonSecPosRang {1,0},///< Filtre, using only this region. Will take into account alignment coordenates.
                    SecPosRang     SecLenLim= SecPosRang    {0,0},///< Limit the length. tiny sec: not created, large: get trunkated
					bool           loadSec  = true                ///< Get the sec? False: get only the dir/file structure
				 ) ;


	    bool	Selected(bool select)	{return _selected=select;} 			///< make protected: ??
	    bool	Selected(		) const {return _selected ;}				///< User-editable

        /// Construct a costum path acording to the current tree, which can be different 
        /// from the original path saved in member variable ._Path
		static std::string	Path(CMultSec *ms, const std::string& path_sep="/")
        {
			std::string path  ;			 
			for (CMultSec *parent=ms; parent;parent=parent->_parentMS)
				path = parent->_name + path_sep + path;
			return path;
        }

        /// Construct a filesystem path acording to the current tree, which can be different 
        /// from the original path saved in member variable ._Path
		std::string	path( )
		{
            std::string sep(std::string(1,filesystem::slash<filesystem::path>().value));
            return Path(this, sep);
		}

		static int			NewMS_ID()
		{
			static int ID(0);
			return ++ID;
		}

		struct CExtremes
		{
		    int			_NSec, _NMSec	;
		    NumRang<LonSecPos> _Len		;		
		    NumRang<Temperature> _Tm	;
		    CExtremes():_NSec(0), _NMSec(0)	{
										    }
		    void Set   (const CSec& s)	{	 
										    _Len.Set(s.Len());		_Tm.Set(s._Tm)	;
									    }
		    void Expand(const CSec& s)	{	
										    _Len.Expand(s.Len());	_Tm.Expand(s._Tm);
									    }
		    void Set   (const CExtremes& e)	{	 
											    _Len.Set(e._Len);		_Tm.Set(e._Tm)	;
										    }
		    bool Expand(const CExtremes& e)	{	
											    bool ex=_Len.Expand(e._Len);  return (_Tm.Expand(e._Tm) || ex );
										    }
		    void Clear(){_NSec=0, _NMSec=0;}

		    bool isExtreme(const CSec&		s){return _Tm.isExtrem( s._Tm ) || _Len.isExtrem( s.Len() );}
		    bool isExtreme(const CExtremes& e){return _Tm.isExtrem( e._Tm ) || _Len.isExtrem( e._Len  );}

		} _Local, _Global;

		void Add2LocalExtreme(const CSec& s)
		{ 
			if (_Global._NSec)
			{
				_Global.Expand(s);
				_Local._NSec ? _Local.Expand(s) : _Local.Set(s);
			}else
			{
				_Local.Set(s);
				_Global.Set(s);
			}
			_Local._NSec++;
			_Global._NSec++;
		}
		void Add2GlobalExtreme(const CSec& s)
		{ 
			if (_Global._NSec)
			{
				_Global.Expand(s);
			}else
			{
				_Global.Set(s);
			}
			_Global._NSec++;
		}
		bool Add2LocalExtreme(const CMultSec& ms)
		{
			_Local._NMSec ++ ;
			return Add2GlobalExtreme(ms);
		}
		bool Add2GlobalExtreme(const CMultSec& ms)
		{	
			_Global._NMSec += 1 + ms._Global._NMSec   ;
			if (! ms._Global._NSec )
				return false;
			bool res=true;
			if (_Global._NSec  )
				res=_Global.Expand(ms._Global); 
			else
				_Global.Set(ms._Global) ;
			_Global._NSec  += ms._Global._NSec   ;
			 return res;
		}
		void RecalExtremes()
		{
			_Local.Clear();

			CSec &to_rest = *CurSec() ; 	
			for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las primeras sec
				Add2LocalExtreme( *CurSec()) ; 
			RestoreCur( &to_rest);
			RecalGlobExtremes();
		}
		void RecalGlobExtremes()
		{
			_Global=_Local;

			CMultSec &to_rest = *CurMSec() ; 	
			for (  goFirstMSec()   ; NotEndMSec()   ;   goNextMSec() )		// recorre todos las primeras sec
				Add2LocalExtreme( *CurMSec()) ; 
			RestoreMCur( &to_rest);
		}
		bool isGlobExtreme(const CSec     *s ){return _Global.isExtreme( *s				) ;}
		bool isGlobExtreme(const CMultSec *ms){return _Global.isExtreme( ms->_Global	) ;}
		bool isLocExtreme (const CSec     *s ){return  _Local.isExtreme( *s				) ;}
		//bool isLocExtreme (const CMultSec *ms){return _Tm.isExtrem (ms->_TTm) || _Len.isExtrem (ms->_TLen );}
		//void setGloExtreme(const CMultSec *ms){return _Tm.isExtrem (ms->_TTm) || _Len.isExtrem (ms->_TLen );}

		//int			AddFromDir		(const std::string& dir , bool  recurs  /*= false*/)
		int		AddFromFile		(const std::string& file);
		int		AddFromFile     (ifstream& ifile);	
		int		AddFromFileFASTA(ifstream &ifileFASTA);
		int		AddFromFileBLAST(ifstream &ifileBLAST);
		int		AddFromFileGB	(ifstream &ifileGB);
		int		AddFromFileGBtxt(ifstream &ifileGB);
		int		AddFromFileODT	(ifstream &ifileODT);
		int		AddFromFileODS	(ifstream &ifileODS);

        /// Reproduce the current -in memory- tree, creating directories as need, 
        /// and export the local sequences in files with extention .fasta.
        /// If the file allready exist create a file with a new name
        /// From each child MSec of this MSec Export one file in fasta format  
        /// (will try all child of base to set the base dir and export)
        /// To decide the name of the file it need the path of the base node that do not form part of the file name
        /// That is for example: "all_seq/Primers for Multiplex PCR/"...
        bool    Export_from   ( CMultSec& base, bool only_selected)  ;

        /// Export the local sequences in a file with extention .fasta.
        /// The name is generated acording to the current postion of the group on the tree.
        /// If the file allready exist create a file with a new name
        /// To decide the name of the file it need the path of the base node that do not form part of the file name
        /// That is for example: "all_seq/Primers for Multiplex PCR/"...
        bool    Export_local_seq   ( CMultSec& base, bool only_selected);

        void   Export_as(std::string filename, bool only_selected)  
        {
        	ofstream ofile( filename ); 
	        if ( ! ofile ) 
	        {
	            throw std::ios_base::failure(string("Could not create the sequence file: ")+ filename );
	        }
            Export( ofile, only_selected);
        }
        void   Export(ofstream& ofile, bool only_selected)
        {
        	for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las sec locales
				if (CurSec()->Selected() || !only_selected)
                    CurSec()->ExportFASTA(ofile) ; 

            for (  goFirstMSec()   ; NotEndMSec() ;   goNextMSec())		// recorre todos las msec
			    CurMSec()->Export(ofile, only_selected);
        }


		CSec		*Idem			(CSec &sec);  //		CConsParam	_ConsPar ;
		CSec		*AddSec			( CSec *sec );
		CSec		*InsertSec		( CSec *sec ) ;
		CSec		*InsertSecAfter	( CSec *sec , CSec *preSec ) ;
		int			CountSelectedSeq		()
		{
            int count{0};
			for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las primeras sec de esta misma ms
				 count += (CurSec()->Selected()) ;
			return count;
		}
		int			CountSelectedSeqRec		()
		{
            int count{0};
				 count += CountSelectedSeq() ;
			for (  goFirstMSec()   ; NotEndMSec()   ;   goNextMSec() )		// recorre todos las primeras sec
				 count += (CurMSec()->CountSelectedSeqRec());
			return count;
		}
		int			CountSelectedNDegSeq	(int MaxGrDeg=0)
		{
			int count(0);
			for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las primeras sec de esta misma ms
				if( CurSec()->Filtered() ) 
					if(CurSec()->NonDegSet())
					{	
						if (!MaxGrDeg)
							count += CurSec()->NonDegSet()->_Global._NSec ;
						else	
							if( MaxGrDeg > CurSec()->NonDegSet()->_Global._NSec)
								count += CurSec()->NonDegSet()->_Global._NSec ;
					}
					else ++count;
			return count;
		}
		int			CountSelectedNDegSeqRec	(int MaxGrDeg=0)
		{
			int count(0);
				 count += CountSelectedNDegSeq( MaxGrDeg) ;
			for (  goFirstMSec()   ; NotEndMSec()   ;   goNextMSec() )		// recorre todos las primeras sec
				 count += CurMSec()->CountSelectedNDegSeqRec( MaxGrDeg);
			return count;
		}

		void			CreateNonDegSet	( )
		{
			for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las primeras sec de esta misma ms
			  CurSec()->CreateNonDegSet();
		}
		void			CreateNonDegSetRec	( )
		{
			CreateNonDegSet( ) ;
			for (  goFirstMSec()   ; NotEndMSec()   ;   goNextMSec() )		// recorre todos las primeras sec
				 CurMSec()->CreateNonDegSetRec( );
		}


		CMultSec	*AddMultiSec	(const std::string &Name )
		{
			return AddMultiSec(new CMultSec (this, Name  ));
		}
		CMultSec	*AddMultiSec	(CMultSec *MultSec);

		CSec		CalculateConsenso	(double) ;
		void		Free			()	{_LSec.free(); _LMSec.free();}
		void		Destroy			()	{_LSec.Destroy(); _LMSec.Destroy();}

		CSec *goFirstSec() {return (CSec *)_LSec.goBeging(); }
		CSec *goNextSec () {return (CSec *)_LSec.goNext  (); }
		CSec *CurSec    () {return (CSec *)_LSec.Cur	 (); }
		CSec *GetLast	() {return (CSec *)_LSec.Last	 (); }
		bool  NotEndSec () {return         _LSec.NotEnd  (); }
		bool     EndSec () {return         _LSec.End     (); }
		void  RestoreCur(CSec *cur){       _LSec.SetCur(cur);}

		CMultSec *goFirstMSec() {return (CMultSec *)_LMSec.goBeging(); }
		CMultSec *goNextMSec () {return (CMultSec *)_LMSec.goNext  (); }
		CMultSec *CurMSec    () {return (CMultSec *)_LMSec.Cur	   (); }
		bool	  NotEndMSec () {return _LMSec.NotEnd  (); }
		void  RestoreMCur(CMultSec *cur){       _LMSec.SetCur(cur);}

		virtual ~CMultSec ()  ;	

        long Len()
           {
               if (_Consenso)
                   return _Consenso->Len();
               else 0;
           }

	private:
        CList			_LSec, _LMSec;    
        //std::list<std::shared_ptr<CSec    >> _LSec;
        //std::list<std::shared_ptr<CMultSec>> _LMSec;
		void			UpdateTotals		( CSec		*sec ) ;
		void			UpdateTotalsAdding	( CSec		*sec ) ;
		void			UpdateTotalsAdding	( CMultSec	*sec ) ;
		CMultSec		*findComParent		( CMultSec	*ms	);
		static void		RefreshExtremes		( CMultSec	*ms	);
};

class CMultAlign   // EXPERIMENTAL
{public:
		CList _LASec;
		long _LenAlign ;
		int _fLenEx;
		void Set_fLenEx(int fLenEx) {_fLenEx=fLenEx;}
		CMultAlign () :  _fLenEx(3){};
		explicit CMultAlign (long LenAlign) : _LenAlign(LenAlign), _fLenEx(3){};
		CMultAlign (CMultSec &MultSec, int fLenEx=3) 
			: _LenAlign(MultSec._Global._Len.Max() *fLenEx), _fLenEx(fLenEx){};//implementsr   !!!!!!!!
	
		void AddFromMultiSec(CMultSec *MultSec);
		void AddSec ( CSec *sec );

};

    // ahora estos seran solo para mantener "copias" (punteros) de sec o msec en otras listas. 
    // Cada sec o msec sabe en que lista esta

class CMSecLink    : public CLink     /// NO es dueno de la sec, no la borra, no delete
{	public:
		CMSecLink (CMultSec *ms, CMSecLink *p, CMSecLink *n=nullptr) : _msec(ms), CLink (n, p) {} ;
		CMultSec *_msec ;
};
#endif


