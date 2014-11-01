#pragma unmanaged	
#ifndef _SEC_H
#define _SEC_H
#include <stdlib.h>
#include <fstream>
#include <cassert>
#include <string>
#include <memory>
#include <vector>


using namespace std;

#include "link.h"
#include "cod_deg.h"
#include "th_dy_param.h"   // crear un nuevo par de fuente cpp con las cosas que nec los dos .h + sec.h
#include "common_basics.h" 

// separar CSec de la ThermDy y crear derivada CThDySec que incluye SdH,S,Tm  ?????????
// anadir funcion de compactar cod (eliminar los gap y bases deg?). SdH y S se recalculan.
// anadir funcion para regenerar cod no compactado, recordar estado comp/no comp
// crear (adaptar) clase primer derivada de CSec, con pos y Tm en cada sec Target
// pensar mejor la relacion con NNPar
// anadir static member PNNParams NNpar??

class CMultSec	;
class ISec				// Pure virtual class ?
{public:			
	using sequence = std::basic_string<Base> ;
	virtual const sequence& Sequence(													)	const=0 ;
	const char*          charSequence()	const {return (char*) (  Sequence().c_str()  );}
	virtual ISec        *Clone   	(DNAStrand strnd=direct								)	const=0 ; /// unique_ptr<ISec> crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
	virtual std::string& Copy_Seq   (std::string &SecHier,  long InicBase, long EndBase, DNAStrand strnd=direct)	const=0  ;
	virtual std::string& Copy_Seq   (std::string &SecHier, DNAStrand strnd=direct)	const=0  ;
	virtual				~ISec			(){}
};


class CSecBasInfo : public ISec
{ protected:	
	bool			_selected{true}, _filtered{false};
	int				_ID{NewS_ID()} ;			                    //< num de la sec en file original?? en total??, num unico?
	std::string     _name ;			                                //< nombre unico? FASTA id	// char			*_name ;
	std::string		_description;
	//long			_len ;			                                //< longitud corregida, sin los '$'
	long			_GrDeg {1} ;		                            //< Grado de degeneracion. Cantidad total de diferentes molec, dependiendo de deg
	float			_GCp   {} ;		
	long			_Count[DegCod::n_dgba];	                        //< An array of counters for each deg base - inicializar!!
	long			_NDB   {};			                            //< cantidad de bases deg
	std::string     _Clas ;		                                    //< clasificacion
	sequence	    _c/*{2,DegCod::basek[DegCod::n_basek]}*/;			//< sec char, comienzan y terminan con '$'0
	CMultSec		*_NonDegSet{nullptr} ;                          /// TODO: std::unique_ptr<>
	static int	NewS_ID     ()	{static int last_ID{};	return ++last_ID;	}

		CSecBasInfo (int id,     const std::string& nam    , const std::string& clas) 						
			           :	_ID	 ( id ), 								
							_name( trim_string	(nam )),	
						    _Clas(clas )/*,
							_c{2,DegCod::basek[DegCod::n_basek]}*/
                       {} 

		CSecBasInfo()  {}
		CSecBasInfo ( long l)    {_c.reserve(l+2);}   ///   ????????????????????
public:
    void ExportFASTA(ofstream& ofile)
    {
        ofile << std::endl << ">" << _name << " " << _description << std::endl << charSequence() << std::endl  ;
    }
	//CSecBasInfo			*CopyFirstBases	(long pos)	;			 // copia parcialmente hasta la pos
    ~CSecBasInfo() override;
    std::string Name		()const		{return _name;}              //< User-editable
	int			ID			()const		{return _ID;}	             //< Run-time-sistem define, non-editable
	bool		Filtered(bool filter)	{return _filtered=filter;}   //< User-editable ?????
	bool		Filtered(		) const {return _filtered;}
	bool		Selected(bool select)	{return _selected=select;} 		  	 //< make protected: ??
	bool		Selected(		) const {return _selected;}					 //< User-editable
	void	    Description (std::string	description)		{ _description=description;}
	virtual std::string	Description ()const	{return !_description.empty() ? _description : Name() ; }

	virtual const sequence& Sequence	(						)	const override   /// ver esto !!!
								{	 
									return _c ;
								}
	 std::string& Copy_Seq  	(std::string &SecHier,  long InicBase, long EndBase, DNAStrand strnd=direct)	const override ;
	 std::string& Copy_Seq  	(std::string &SecHier,  DNAStrand strnd=direct)	const override {return Copy_Seq ( SecHier, 1, Len(), strnd ) ;}
	 bool		 NotIdem		(CSecBasInfo *sec) {return false;}
	Base		operator[]	(int i)const{return _c[i];}  /// i+1 ????

	long		Len			()const		{return _c.length()-2;} //
	long		Degeneracy	()const		{return _GrDeg;}
	long		*BaseCount	()			{return _Count;}
	long		BaseCount	(Base b)	{ return  DegCod::is_degbase[b] ?  _Count[DegCod::db2nu[b]] : 0;}
	CMultSec	*NonDegSet	()			{return _NonDegSet;}
	float		GCpercent	()const		{return	_GCp ;}		
};


class CSec : public CLink, public CSecBasInfo	// ---------------------------------------   CSec	---------------------------------------------------
{public:
	    int                     x;				//<  ????
		NumRang<float>	        _Tm ;			//< float		_Tm, _minTm, _maxTm ;				//  
		std::shared_ptr<CSaltCorrNN>  _NNpar ;
		float			        _Conc ;			//< conc de esta molec. Si igual al resto -1 y la toma de NNParam
		std::vector<Code>	    _b;			//< sec cod, inicialmente basek
		std::vector<Entropy>    _SdS ;			//< dS acumulada. Calcular Delta S sera solo restar la final menos la inicial	
		std::vector<Energy>		_SdH ;			// 
		CMultSec	           *_parentMS{nullptr}	;   //std::weak_ptr<CMultSec> _parentMS	;

	CSec (  const std::string&  sec, 
		    int                 id,
            const std::string&  nam,     // char*
            std::shared_ptr<CSaltCorrNN>  NNpar, 
            long                lmax=0, //< limita la cant de bases originales a leer despues de las primeras secBeg-1 bases 
            long                secBeg=1, 
            const std::string&  clas="" , 
            float               conc=-1         );
	CSec ( long l, std::shared_ptr<CSaltCorrNN>  NNpar) ;


	long		Len			()const		{return _SdS.size();} //
	void		 CorrectSaltOwczarzy    () ;
	CMultSec	*CreateNonDegSet		()			;   //< crea todo el set si no existia, solo si existen bases deg: _NDB>0
	CMultSec	*ForceNonDegSet			();				//< lo crea siempre, incluso para =1??
	CSec		*GenerateNonDegVariant	(CSec *s, long pos, Base ndb)   ; //< recursiva
	CSec		*CopyFirstBases			(long pos)	;			//< copia parcialmente hasta la pos
	void		 CorrectSalt			() { if ( _NNpar->UseOwczarzy () ) CorrectSaltOwczarzy();};
	CSec		*Clone   	(DNAStrand strnd=direct	 ) const override; /// unique_ptr<ISec> crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados

	//virtual CSec*CreateCopy		(DNAStrand strnd=direct) override;//< crea una copia muy simple. CUIDADO con copias de CSecBLASTHit y otros derivados
	//const char	*Get_charSec			()const{return (const char*)_c.c_str();}  ///   ???????????

    bool		Selected() const;				 //< User-editable    ???????????????????????????????????????????????????????????????????????????
    bool		Selected(bool select)	{ _selected = select; return Selected(); } 			//< make protected: ?????????????????????????????????????????????????

	Base		operator()		(int i)const{return _b[i];}
	Temperature	Tm	(long pi, long pf	)const;				        //< Tm de la sonda con sec desde pi hasta pf, inclusive ambas!! 
	Temperature	Tm	(long pi			)const	{return Tm(pi,Len())   ;}   //< Tm de la sonda con sec desde pi hasta el final, inclusive ambos!!
	Energy		G	(long pi, long pf, float Ta)const;				//< G de la sonda con sec desde pi hasta pf, inclusive ambas!! 
	Energy		G	(long pi, float Ta	)const	{return G(pi,Len(), Ta);}   //< G de la sonda con sec desde pi hasta el final, inclusive ambos!!
	Energy		G	(float Ta			)const	{return G(1 ,Len(), Ta);}   //< G de la sonda con sec desde inicio hasta el final, inclusive ambos!!
	Energy		G	(long pi, long pf	)const;				//< G de la sonda con sec desde pi hasta pf, inclusive ambas!! 
	Energy		G	(long pi			)const	{return G(pi,Len())    ;}   //< G de la sonda con sec desde pi hasta el final, inclusive ambos!!
	Energy		G	(					)const	{return G(1,Len())     ;}   //< G de la sonda con sec desde inicio hasta el final, inclusive ambos!!

	 ~CSec() override  ;   // decidir si vale la pena que sea virtual. Cual es el efecto??
	virtual bool NotIdem(CSec *sec) {return false;}
};

      //<Hsp_num>1</Hsp_num>
      //<Hsp_bit-score>482.786</Hsp_bit-score>
      //<Hsp_score>534</Hsp_score>
      //<Hsp_evalue>3.71782e-133</Hsp_evalue>
      //<Hsp_query-from>1</Hsp_query-from>
      //<Hsp_query-to>267</Hsp_query-to>
      //<Hsp_hit-from>9043</Hsp_hit-from>
      //<Hsp_hit-to>9309</Hsp_hit-to>
      //<Hsp_query-frame>1</>
      //<Hsp_hit-frame>1</>
      //<Hsp_identity>267</>
      //<Hsp_positive>267</>
      //<Hsp_gaps>0</>
      //<Hsp_align-len>267</>
      //<Hsp_qseq>TACAACATGATGGGAAAGAGAGAGAAGAAGCCTGGAGAGTTCGGCAAGGCTAAAGGCAGCAGAGCCATCTGGTTCATGTGGCTGGGGGCTCGCTTCCTGGAGTTTGAAGCTCTCGGATTCCTCAATGAAGACCACTGGCTGGGTAGGAAGAACTCAGGAGGAGGAGTAGAAGGCTTAGGACTGCAGAAGCTTGGGTACATCTTGAAGGAAGTTGGGACAAAGCCTGGAGGAAAGATTTACGCTGATGATACCGCAGGCTGGGACACA</Hsp_qseq>
      //<Hsp_hseq>TACAACATGATGGGAAAGAGAGAGAAGAAGCCTGGAGAGTTCGGCAAGGCTAAAGGCAGCAGAGCCATCTGGTTCATGTGGCTGGGGGCTCGCTTCCTGGAGTTTGAAGCTCTCGGATTCCTCAATGAAGACCACTGGCTGGGTAGGAAGAACTCAGGAGGAGGAGTAGAAGGCTTAGGACTGCAGAAGCTTGGGTACATCTTGAAGGAAGTTGGGACAAAGCCTGGAGGAAAGATTTACGCTGATGATACCGCAGGCTGGGACACA</Hsp_hseq>
      //<>
class CSecBLASTHit : public CSec // ---------------------------------------   CSecBLASTHit	------------------------------------------------
{public:
	CSecBLASTHit(	unsigned int	BlastOutput_query_len ,
					// para cada hit
					unsigned int	Hit_num ,
					std::string&&   Hit_id ,				
					std::string&&   Hit_def ,				
					std::string&&   Hit_accession	,
					long			Hit_len ,				
					float			Hsp_bit_score ,
					unsigned int	Hsp_score ,
					double			Hsp_evalue ,
					unsigned int	Hsp_query_from ,
					unsigned int	Hsp_query_to ,
					unsigned int	Hsp_hit_from ,
					unsigned int	Hsp_hit_to ,
					unsigned int	Hsp_query_frame ,
					unsigned int	Hsp_hit_frame ,
					unsigned int	Hsp_identity ,
					unsigned int	Hsp_positive ,
					unsigned int	Hsp_gaps ,
					unsigned int	Hsp_align_len ,
					std::string&&   Hsp_midline ,
					bool			FormatOK ,
					std::string&&   sec	,	
					NumRang<long>	SecLim,			                 //long	SecBeg,long	SecEnd,
					int				id,				                 //	Hit_num	char	*	nam,	Hit_def
                    std::shared_ptr<CSaltCorrNN>  NNpar,			 //	long  l=0,	Hit_len ------> _Hsp_align_len
					std::string	    clas="", 
					float			conc=-1
				)  :
						CSec (  std::move(sec),   
                                id,   
                                std::move(Hit_accession),   
                                NNpar, 
								(SecLim.Max() && long(Hsp_query_to) > SecLim.Max()   ) ? SecLim.Max()       - SecLim.Min() +1 
                                                                                       : long(Hsp_query_to) - SecLim.Min() +1	,	//Hsp_align_len,  --  Long
								SecLim.Min() - long(Hsp_query_from+1) ,                 //     SecBeg
								clas,   
                                conc ),
							_BlastOutput_query_len( BlastOutput_query_len ) ,
							// para cada hit
							_Hit_num		( Hit_num ) ,
							_Hit_id			( std::move(Hit_id) ) ,				
							_Hit_def		( std::move(Hit_def) ) ,				
							_Hit_accession	( std::move(Hit_accession) )	,
							_Hit_len		( Hit_len ) ,				
							_Hsp_bit_score	( Hsp_bit_score ) ,
							_Hsp_score		( Hsp_score ) ,
							_Hsp_evalue		( Hsp_evalue ) ,
							_Hsp_query_from	( Hsp_query_from ) ,
							_Hsp_query_to	( Hsp_query_to ) ,
							_Hsp_hit_from	( Hsp_hit_from ) ,
							_Hsp_hit_to		( Hsp_hit_to ) ,
							_Hsp_query_frame( Hsp_query_frame ) ,
							_Hsp_hit_frame	( Hsp_hit_frame ) ,
							_Hsp_identity	( Hsp_identity ) ,
							_Hsp_positive	( Hsp_positive ) ,
							_Hsp_gaps		( Hsp_gaps ) ,
							_Hsp_align_len	( Hsp_align_len ) ,
							_Hsp_midline	( std::move(Hsp_midline) ) ,
							_FormatOK		( FormatOK ) ,
							_SecLim			( SecLim )	/*,_SecBeg	(SecBeg), 	_SecEnd		(SecEnd)*/
							{
                        }			


	unsigned int	_BlastOutput_query_len ;
	// para cada hit
	unsigned int	_Hit_num ;
	std::string	    _Hit_id ;				
	std::string	    _Hit_def ;				
	std::string	    _Hit_accession	;
	long			_Hit_len ;				
	float			_Hsp_bit_score ;
	unsigned int	_Hsp_score ;
	double			_Hsp_evalue ;
	unsigned int	_Hsp_query_from ;
	unsigned int	_Hsp_query_to ;
	unsigned int	_Hsp_hit_from ;
	unsigned int	_Hsp_hit_to ;
	unsigned int	_Hsp_query_frame ;
	unsigned int	_Hsp_hit_frame ;
	unsigned int	_Hsp_identity ;
	unsigned int	_Hsp_positive ;
	unsigned int	_Hsp_gaps ;
	unsigned int	_Hsp_align_len ;
	std::string	    _Hsp_midline ;
	bool			_FormatOK ;
	NumRang<long>	_SecLim;   	                              //long	_SecBeg;	//long	_SecEnd;
	std::string	Description ()const	override {return _description.length() ? _description : _Hit_def ; }
};

class CSecGB : public CSec // ---------------------------------------   CSecGB	------------------------------------------------
{public:
	std::string 	_Textseq_id_accession ;	
	std::string 	_Org_ref_taxname	 ;
	std::string 	_Seqdesc_title		 ;
	long			_Seq_inst_length	 ;	    
	
	CSecGB(			std::string&&	Textseq_id_accession ,	
					std::string&&	Org_ref_taxname	 ,
					std::string&&	Seqdesc_title	,
					long			Seq_inst_length	 ,    
					const char	*	sec	,	
					int				id,						//	char		*	nam,	Seqdesc_title	,	
					std::shared_ptr<CSaltCorrNN>  NNpar,  				//	long			l=0,		Seq_inst_length
					std::string 	clas="", 
					float			conc=-1
				):
						CSec (sec, id, Textseq_id_accession, NNpar, Seq_inst_length,1, clas, conc ),// actualizar Beg-End

										_Textseq_id_accession	( std::move(Textseq_id_accession) ) ,
										_Org_ref_taxname		( std::move(Org_ref_taxname )) ,
										_Seqdesc_title			( std::move(Seqdesc_title) ) ,				
										_Seq_inst_length		( Seq_inst_length ) 				{}			
	virtual std::string	Description ()const override	{return _description.length() ? _description : _Seqdesc_title ; }

	virtual ~CSecGB(){	}
};

class CSecGBtxt : public CSec // ---------------------------------------   CSecGBtxt	------------------------------------------------
{public:
	std::string	_LOCUS			;
	long		_Seq_inst_length;	
	std::string	_DEFINITION      ;
	std::string	_ACCESSION       ;
	std::string	_ORGANISM        ;			// ORIGIN      
	CSecGBtxt(		std::string&&	LOCUS       ,
					long		    Seq_inst_length,	
					std::string&&	DEFINITION     ,
					std::string&&	ACCESSION      ,
					std::string&&	ORGANISM       ,
					const char	*	sec	,	
					int				id,						//	char		*	nam,	DEFINITION	,	
					std::shared_ptr<CSaltCorrNN>  NNpar,  				//	long			l=0,		Seq_inst_length
					std::string	    clas="", 
					float			conc=-1
				):		CSec (sec, id, LOCUS, NNpar, 0,1, clas, conc ), // actualizar Beg-End
							_LOCUS			( std::move(LOCUS )) ,
							_Seq_inst_length( Seq_inst_length ) ,
							_DEFINITION		( std::move(DEFINITION) ) ,				
							_ACCESSION		( std::move(ACCESSION) ) 	,			
							_ORGANISM		( std::move(ORGANISM )) 				{}	
	virtual std::string	Description ()const override	{return _description.length() ? _description : _DEFINITION ; }
	
	virtual ~CSecGBtxt() {				}	
};


class CRangBase : public NumRang<long> // ---------------------------------------   CRang	: AMPLIAR y mejorar !!!  ---------------------------------------
{public:	
	CRangBase (long i,long f) : NumRang<long>(i,f), _c(   f+1, i-1   )  { /* open();*/} 		// NumRang<long> _p;

public:
	CRangBase MatchRange() {return CRangBase( _c.Min(), _c.Max() );} // NO ME GUSTA ASI  !!!!!! pensar algo mas eficiente
	void open(void){  _c.Set(   Max()+1, Min()-1   )     ;}
						//    pi      pf                fi
											//----|++++++++|-----------------|--------			El rango inicial, y como va quedando
											// pfcur                        fi					El rango para calculo ("cur"), antes del comienzo	
											//---|----------|----------------|--------			Asi se queda si no hibridan entre si las sec en esta zona,
											//             picur            fi					y entonces "colapsa" el rango
											//            pfcur             fi					En este caso encontro 5 "cand" comunes	
											//--------|+++|------------------|--------			
											//       picur                  fi					
	//NumRang<long>	_p, _pcur ;
	void adjustCur(long p){ _c.Expand(p); } //if( _c.Min()>p ) 	_c.Min()=p;		if( _c.Max()<p )	_c.Max()=p;}
	bool isOpen  ()const{ return _c.Min() > _c.Max() ;}
	bool hasMatch()const{ return !isOpen   ()    ;}
	long NumMatch()const{ return _c.length() + 1;}
	void SchrinkToMatch(){Set(_c);}					// scheck if open????  
	//long length()const  { return Max() - Min() ;}
	void schift(int s) { Min()+=s;Max()+=s;_c.Min()+=s;_c.Max()+=s;}   //{ _pf+=s;_pi+=s;_pfcur+=s;_picur+=s;}
	bool addMatch(long i){ if (inRang(i)) {adjustCur(i);return true;} else return false;}

 protected:
	NumRang<long> _c;		//	long			_pi,_picur, _pf,_pfcur; //  _picur= _pf+1; _pfcur= _pi-1;}	
} ; 
class CRangBaseSchift /*: public CRangBase */
{	CRangBase &_R;
	long _sch;
public:
	CRangBaseSchift	(CRangBase& r, long sch) : _R(r), _sch(sch)
				{
					_R.schift(_sch);
				}
	void AddSchift	(int sch)
				{
					_sch+=sch;
					_R.schift(sch);
				}
	void ResetSchift()
				{
					_R.schift(-_sch);
					_sch=0;
				}
		~CRangBaseSchift()
				{
					ResetSchift();
				}
};



class CRang : public CRangBase// ---------------------------------------   CRang	: AMPLIAR y mejorar !!!  ---------------------------------------
{public:		//NumRang<long>	_p, _pcur ;

	       CRang (long i,long f) : CRangBase ( i, f),	 matchs(new int[length()+1])    { for (int i=0 ; i<= length() ; ++i) matchs[i]=0;} 
		   CRang (CRangBase  &R) : CRangBase ( R ),	     matchs(new int[length()+1])    { for (int i=0 ; i<= length() ; ++i) matchs[i]=0;} 
	int	   *matchs;
	      ~CRang  ( )  { delete []matchs; }
    void  IncrMatchs() { 	for (int pi_pos=_c.Min()    ; pi_pos <= _c.Max() ; pi_pos++ ) 	matchs[pi_pos - Min() ]++;		}

} ; typedef CRang *pCRang ;


//! destinado a formar parte de una lista en un busq de sondas.-------------------------   CSecCand	-------------------------------
class CSecCand : public CLink 
{public:						
	long _NumPosCand, _NumCand  , _NumPosCandIn, _NumCandIn  ,_NumCandExact ;	
	CSec &_Sec ;	// ref a la sec, que ni se modifica ni se cambia de lugar
	pCRang *_rg;

	//CSecCand(CSec &sec, float Tm_min, float Tm_max, int L_min, int L_max);
	CSecCand(CSec &sec, 	SondeLimits sL		);
							//float	G_min	, float G_max ,					// en kcal ...
							//float	Tm_min	, float Tm_max ,  
							//int		L_min	, int L_max 

	long ColapseRangs(bool colapse=true);

	virtual ~CSecCand(){ for (long i=0; i<=_Sec.Len(); i++)	delete _rg[i]; 	delete _rg;}
};


class CSecAl : public CLink // destinado a formar parte de una lista en un alineamieto.
{public:
	CSec &_Sec ;	// ref a la sec, que ni se modifica ni se cambia de lugar
	long *_inAlp_B ;// array que dice que base de la sec va en esa pos del Al (len=Al)
	long *_inBp_Al ;// array que dice en que pos del Al va esa base de la sec (len=sec)

	CSecAl(CSec &sec, long LenAlign)
		: _Sec(sec), 
		  _inAlp_B(new long[sec.Len()]), 
		  _inBp_Al(new long[LenAlign])
	   {}
	CSecAl(CSec &sec) 
		: _Sec(sec), 
		  _inAlp_B(new long[sec.Len()]), 
		  _inBp_Al(0)
	  {}
	virtual ~CSecAl(){ delete []_inAlp_B; delete []_inBp_Al;}

	char *CopyAlignedSecChar(long Al_pBeg, long Al_pEnd, char *CharSec)	;// CUIDADO !! asume suficiente espacio !!
	char *GetAlignedSecChar (long Al_pBeg, long Al_pEnd);  // "regala" esta memoria, no olvide delete !
};

  // permite hacer grupos de sec o de MultiSec para analisis por "especies"
  // anadir posibilidad de construir sec concenso 
  // CUIDADO :  se aduena de las sec y las borra en su destructor:Usar Remove() or Free() para evitarlo
class CMultSec	 : public CLink	// --------------------------------------------------------------------- 	CMultSec    -------------------
{	public:
		std::string			_name ;						///< 
        int					_ID       {NewMS_ID()};		///< Unique ID in each programm run
        LonSecPosRang       _SecLim   {1,0};			///< TODO: quitar de aqui?. Pertenece a CSec, o a un objeto "AddFromFile" 
        SecPosRang          _SecLenLim{0,0};            ///< TODO: NumRang<LonSecPos> _SecRange{1,0};// def-unlimited: the range used, not the limits
		float				_MaxTgId  {100};			///< TODO: quitar de aqui?. Pertenece a CSec, o a un objeto "AddFromFile" 
		std::shared_ptr<CSaltCorrNN>	_NNPar ;		///< TODO: quitar de aqui?. Pertenece a CSec, o a un objeto "AddFromFile" 
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
                                                                _NNPar      (ms->_NNPar)         {  }
		 CMultSec (	ifstream &	 file	,	 
					std::shared_ptr<CSaltCorrNN>  NNpar	, 
					float		  MaxTgId	= 100, 
					LonSecPosRang  SecLim	= LonSecPosRang {1,0}, 
                    SecPosRang     SecLenLim= SecPosRang{0,0})  : /*_name(trim_string(file)),	*/
	                                                            _SecLim     (SecLim),
                                                                _SecLenLim  (SecLenLim),
	                                                            _MaxTgId    (MaxTgId), 
	                                                            _NNPar      (NNpar)              { AddFromFile(file); }
		 CMultSec (	const std::string &file	,                   //< The name of the file or directory to be loaded 
					std::shared_ptr<CSaltCorrNN>  NNpar	, 
					bool           all_dir  = false,        //< Load all files and directories recursiverly? 
					float		   MaxTgId	= 100,          //< Sec. with more % of idem are marked as "filtered" and not selected
					LonSecPosRang  SecLim	= LonSecPosRang {1,0},	//< Filtre, using only this region. Will take into account alignment coordenates.
                    SecPosRang     SecLenLim= SecPosRang    {0,0},  //< Limit the length. tiny sec: not created, large: get trunkated
					bool           loadSec  = true     //< Get the sec? False: get only the dir/file structure
				 ) ;




	bool		Selected(bool select)	{return _selected=select;} 			///< make protected: ??
	bool		Selected(		) const {return _selected ;}					 ///< User-editable
		//std::string Path(const std::string& path_sep="/")
		//{
		//	std::string path /*= _name*/;			// anadir o no un sep al final del path?????
		//	for (CMultSec *parent=this; parent;parent=parent->_parentMS)
		//		path = parent->_name + path_sep + path;
		//	return path;
		//}
		static std::string	Path(CMultSec *ms, const std::string& path_sep="/")
		{
			std::string path /*= _name*/;			/// anadir o no un sep al final del path?????
			for (CMultSec *parent=ms; parent;parent=parent->_parentMS)
				path = parent->_name + path_sep + path;
			return path;
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
		int			AddFromFile		(const std::string& file);
		int			AddFromFile     (ifstream& ifile);	
		int			CMultSec::AddFromFileFASTA	(ifstream &ifileFASTA);
		int			CMultSec::AddFromFileBLAST	(ifstream &ifileBLAST);
		int			CMultSec::AddFromFileGB		(ifstream &ifileGB);
		int			CMultSec::AddFromFileGBtxt	(ifstream &ifileGB);
		int			CMultSec::AddFromFileODT	(ifstream &ifileODT);
		int			CMultSec::AddFromFileODS	(ifstream &ifileODS);
        void        ExportFASTA(std::string filename, bool only_selected)
        {
        	ofstream ofile( filename ); 
	        if ( ! ofile ) 
	        {
	            throw std::ios_base::failure(string("Could not create the sequence file: ")+ filename );
	        }
            FASTA( ofile, only_selected);
        }
        void   FASTA(ofstream& ofile, bool only_selected)
        {
        	for (  goFirstSec()   ; NotEndSec()   ;   goNextSec() )		// recorre todos las sec locales
				CurSec()->ExportFASTA(ofile) ; 
            for (  goFirstMSec()   ; NotEndMSec() ;   goNextMSec())		// recorre todos las msec
			    CurMSec()->FASTA(ofile, only_selected);
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

class CSecLink   : public CLink    // NO es dueno de la sec, no la borra, no delete
{	public:
		CSecLink (CSec *s, CSecLink *p, CSecLink *n=nullptr) : _sec(s), CLink (n, p) {} ;
		CSec *_sec ;
};
		
class CMSecLink    : public CLink     // NO es dueno de la sec, no la borra, no delete
{	public:
		CMSecLink (CMultSec *ms, CMSecLink *p, CMSecLink *n=nullptr) : _msec(ms), CLink (n, p) {} ;
		CMultSec *_msec ;
};
#endif
//class CConsParam 
//{	public:
//		double _min ;
//};
//enum DNAStrand						{plus	, minus, direct	, rev	, compl, rev_compl	} ;
//// extern char *DNAStrandName[]=	{""		, "(c)", ""		, "(r)"	, "(i)", "(c)"		} ;
//extern char *DNAStrandName[];
//enum fileFormat {fasta=1 , csv=1<<1, f2=1<<2, f3=1<<3} ; // se pueden "OR" los formatos : OUTPUT !!!!!!
//
//char *AttachToCharStr       (const char *CharStr, const char *Attach)	;
//char *ChangeCharStrAttaching(char *&CharStrToChange, const char *Attach); // CharStrToChange : debe ser una cadena que se creo con new, 
//char *ChangeCharStrAttaching(char *&CharStrToChange, const int Attach);// y que sera borrada y vuelta a crear !!!
