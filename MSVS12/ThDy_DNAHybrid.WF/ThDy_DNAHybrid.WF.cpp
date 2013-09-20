// ThDy_DNAHybrid.WF.cpp : main project file.
#include "stdafx.h"
#include "ThDy_Form.h"

//#include "ThDy_DNAHybrid.VC-WF/tpForm.h"

// tpF.cpp : main project file.
// thermodynamics, hibridación  hybridization  

namespace TagBindingNS 
{
//CParamBool
    ref		  class TagBinding_CParamBool : public TagBinding //    Bind a Control.CheckBox with a bool variable ---- TagBinding_bool    :
    { 	protected:	CParamBool	&_p ;
	    public:				TagBinding_CParamBool (CParamBool &p, CheckBox^ c):TagBinding(c), _p(p){SetDef()		;} 

	    virtual void		set(bool p	 )			{						_p.set( p )	; }// OJO  nuevas get y set !!! no las virtuales heredadas
	    virtual	bool		get(		 )	new		{ return  _p.get()					; }
	    virtual void		UpDateForm(	 )	override{ ((CheckBox^ )_c)->Checked = get()	;}
	    virtual void		UpDateP(	 )	override{ set (((CheckBox^ )_c)->Checked)	;}
    };
    TagBinding_CParamBool^	TagBind(CheckBox^	  c , CParamBool  &p			){ return gcnew TagBinding_CParamBool(p,c)	;}


}

using namespace TagBindingNS ;
using namespace ThDy_DNAHybridWF;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew ThPr_Form());
	return 0;
}
void 		ThPr_Form::InitializeTagBindings()
{
	_CommThDyP = gcnew	TagBindGroup() 
		    <<	/*PriorizeDefault::*/TagBindingNS::Form		//   PriorizeDefault::Parametr
			<<		TagBind_Trim		( textBoxNNParamFile			, _Pr._cp._InputNNFile		)		
			<<		TagBind_Trim		( textBoxTgScF4uA				, _Pr._cp._InputTargetFile	)		
			<<		TagBind_Trim		( textBoxSaveResultFile			, _Pr._cp._OutputFile		)	
			<<		TagBind 			( numUpDowTa					, _Pr._cp._Ta				)				
			<<		TagBind				( numUpDowSdConc				, _Pr._cp._ConcSd,	1e-9f	)
			<<		TagBind				( numUpDowTgConc				, _Pr._cp._ConcTg,	1e-9f	)
			<<		TagBind				( numUpDowSalConc				, _Pr._cp._ConcSalt,1e-3f	)
			<<		TagBind				( numUpDwMaxTgId				, _Pr._cp._MaxTgId			)
			<<		TagBind				( ckBx_savNNParam				, _Pr._cp._saveNNPar		)  
			<<		TagBind				( ckBx_loadNNParam				, _Pr._cp._loadNNPar		)  
			<<		TagBind				( ckBx_savTm					, _Pr._cp._st_savTm			)  
			<<		TagBind				( ckBx_savPos					, _Pr._cp._st_savPos		)  
			<<		TagBind				( ckBx_savG						, _Pr._cp._st_savG			)  
			<<		TagBind				( ckBx_savAlign					, _Pr._cp._st_savAlign		)  
			<<		TagBind				( ckBx_savProj					, _Pr._cp._st_savProj		)  
			<<		TagBind				( ckBx_savG_Plasm				, _Pr._cp._st_savG_Plasm	)  
			<<		TagBind				( ckBx_savTm_Plasm				, _Pr._cp._st_savTm_Plasm	)  
			<<		TagBind				( ckBx_savLog					, _Pr._cp._st_savLog		)  
			<<		TagBind				( ckBx_savExportSond			, _Pr._cp._st_Exp_sond		)  
			<<		TagBind				( ckBx_savExportTarg			, _Pr._cp._st_ExpTarg		)  
			<<		TagBind				( numUpDw_MinLen				, _Pr._cp._MinSecLen		)  
			<<		TagBind 			( numUpDw_TgBeg, numUpDw_TgEnd	, _Pr._cp._SecLim			)	

			;

	_uArrThDyP  = gcnew	TagBindGroup()
			<<		TagBind_Trim		( textBoxSondesFile	, _Pr._uArr._InputSondeFile		)	
			;

	_mPCRThDyP  = gcnew	TagBindGroup()
			<<		TagBind_Trim		( textBoxSdSecFilePCR, _Pr._mPCR._InputSondeFile	)	
			;


	_SdDesThDyP = gcnew	TagBindGroup()
			<<		TagBind 		( nUpDowMaxSd_TgG						, _Pr._SdDes._G_sig				)				
			<<		TagBind 		( nUpDowMinSd_TgTm						, _Pr._SdDes._Tm_sig			)				
			<<		TagBind 		( nUpDowMinSd_nonTgG					, _Pr._SdDes._MinSd_nTgG		)				
			<<		TagBind 		( nUpDowMaxSd_nonTgTm					, _Pr._SdDes._MaxSd_nTgTm		)				
			<<		TagBind 		( numUpDwMinSelfG						, _Pr._SdDes._MinSelfG			)				
			<<		TagBind 		( numUpDwMaxSelfTm						, _Pr._SdDes._MaxSelfTm			)				
			<<		TagBind		( chkBx_common		 , _Pr._SdDes.common	)  
			<<		TagBind		( chkBx_unique		 , _Pr._SdDes.unique	)  
			<<		TagBind 	( numUpDw_MinTargCov,		numUpDw_MaxTargCov			, _Pr._SdDes.Coverage.getRef ()			)	
			<<		TagBind 		(nUpDw_MinSd_G		,nUpDw_MaxSd_G		,_Pr._SdDes._sL._G				)	
			<<		TagBind 		(nUpDowMinSdTm		,nUpDowMaxSdTm		,_Pr._SdDes._sL._Tm				)	
			<<		TagBind 		(nUpDw_MinSdLength	,nUpDw_MaxSdLength	,_Pr._SdDes._sL._L				)	
			;

 /*                 << link (  _Pr._SdDes.common,           chkBx_common)
                  << link (  _Pr._SdDes.unique,           chkBx_unique)
                  << link ( _Pr._SdDes.Coverage,  numUpDw_MaxTargCov,  numUpDw_MinTargCov)	
*/



	_TmCalThDyP = gcnew	TagBindGroup()
			<<		TagBind 	( txtBx_Sec			 , _Pr._TmCal._Sec		)	
			<<		TagBind 	( txtBx_Sec2Align	 , _Pr._TmCal._Sec2Align)	
			<<		TagBind 	( chkBx_Tm_save_asPCR, _Pr._TmCal._save		)
			<<		TagBind		( chkBx_align		 , _Pr._TmCal._align	)  
			;

	this->dGVw_TmCalcRes->RowCount	= 3;				// create rows
	this->dGVw_TmCalcRes->Rows[0]->HeaderCell->Value =  "Up" ;
	this->dGVw_TmCalcRes->Rows[1]->HeaderCell->Value =  "Down" ;
	this->dGVw_TmCalcRes->Rows[2]->HeaderCell->Value =  "Interact" ;




}


