/**
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2015
*
* @file  ThDySec\include\ThDy_DNAHybrid.Nana\main.Nana.h
*
* @brief 
*/

#ifndef SetupPage_H
#define SetupPage_H

//#include "thdy_programs\init_thdy_prog_param.h"
//#include "matrix.h" 
//#include "common_basics.h" 

#include <../../nana.ext/include/EditableForm.hpp>
#include <../../nana.ext/include/Numer.hpp>
#include "../../nana.ext/include/nanaBind.hpp"

#include <nana/gui/widgets/group.hpp>
//#include <nana/gui/widgets/progress.hpp>
//#include <nana/gui/widgets/tabbar.hpp>
//#include <nana/gui/widgets/checkbox.hpp>
//#include <nana/gui/widgets/treebox.hpp>
//#include <nana/gui/widgets/listbox.hpp>
//#include <nana/gui/widgets/toolbar.hpp>
//#include <nana/gui/tooltip.hpp>
//#include <nana/gui/wvl.hpp>

//#include <iostream>    // temp, for debugging
//#include <fstream>     // temp, for debugging
//#include <filesystem>



class ThDyNanaForm ;
 

class SetupPage : public CompoWidget
{
    ThDyNanaForm       &_Pr;
    nana::group         _gr_dir  {*this, STR(" <bold=true> Directories: </>"          ), true};
    FilePickBox         _results            { _gr_dir.inner(), STR("Results: ") } ;

    nana::group         _gr_seq         {_gr_dir.inner(), STR(" <bold=true> Sequences </>File or Directory"      ), true};
    nana::group         _gr_targ        {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _targets            { _gr_targ.inner(), STR("Targets: ") }  ;
    nana::checkbox      _chkTargRecDir      { _gr_targ.inner(), STR("Targets - Recur Dir") },
                        _chkTargOnlyStruct  { _gr_targ.inner(), STR("Only reproduce Dir Structure") };

    nana::group         _gr_ntarg       {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _nTsec              { _gr_ntarg.inner(), STR("Non targets: "),STR("FindSonden-OSB.NonTarg.lay.txt")};
    nana::checkbox      _chk_nTgRecDir      { _gr_ntarg.inner(), STR("Non Targets - Recur Dir") },
                        _chk_nTgOnlyStruct  { _gr_ntarg.inner(), STR("Only reproduce Dir Structure") };

    nana::group         _gr_PCRfiltre       {_gr_seq.inner(), STR(""                                             ), true};
    FilePickBox         _PCRfiltre          { _gr_PCRfiltre.inner(), STR("PCR-filtre: ")};

    nana::group         _gr_PrimersFilePCR  {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _PrimersFilePCR     { _gr_PrimersFilePCR.inner(), STR("Primers: ") };
    nana::checkbox      _chkPrimRecDir      { _gr_PrimersFilePCR.inner(), STR("Primers - Recur Dir") },
                        _chkPrOnlyStruct    { _gr_PrimersFilePCR.inner(), STR("Only reproduce Dir Structure") };

    nana::group         _gr_uArr        {_gr_seq.inner(), STR(""                                                 ), true};
    FilePickBox         _Prob_uArr          { _gr_uArr.inner(), STR("Probes: ") };
    nana::checkbox      _chkProbRecDir      { _gr_uArr.inner(), STR("Probes - Recur Dir") },
                        _chkProbOnlyStruct  { _gr_uArr.inner(), STR("Only reproduce Dir Structure") };

    OpenSaveBox         _NNParamFile        { _gr_dir.inner(), STR("NN param: ")};

    nana::group         _gr_salt {*this, STR(" <bold=true> Input & analisis parameters: </>"                 ), true};
    nana::combox        comBoxSalMeth       { _gr_salt.inner()}, 
                        comBoxTAMeth        { _gr_salt.inner()};
    nana::NumUnitUpDown numUpDowTgConc      { _gr_salt.inner(), STR("Target Conctr:"      ), 50, 0.1    , 1000000,  "nM"}, 
                        numUpDowSalConc     { _gr_salt.inner(), STR("Salt Conc [Cations]:"), 50, 0.0001 , 10000,    "mM"} , 
                        numUpDowTa          { _gr_salt.inner(), STR("Temp. Anneling:"     ), 55, 40     , 75,       "°C"},  
                        numUpDowSdConc      { _gr_salt.inner(), STR("Sonde Conctr:"       ), 0.8, 0.001 , 1000,     "µM"}  ;

    nana::button        _set_def_proj       { *this,STR("Set as Def. project") },
                        _load_def_proj      { *this,STR("ReLoad Def. project") };

    nana::group         _gr_checks {*this, STR(" <bold=true> Save in results: </>"                 ), true};
    nana::checkbox      ckBx_savTm          { _gr_checks.inner(), STR("Tm"    ) },
                        ckBx_savPos         { _gr_checks.inner(), STR("Pos"   ) },
                        ckBx_savG           { _gr_checks.inner(), STR("G"     ) },
                        ckBx_savAlign       { _gr_checks.inner(), STR("Align" ) },
                        ckBx_savProj        { _gr_checks.inner(), STR("Proj"  ) },
                        ckBx_savG_Plasm     { _gr_checks.inner(), STR("G->Plasmid") },
                        ckBx_savTm_Plasm    { _gr_checks.inner(), STR("Tm->Plasmid") },
                        ckBx_savLog         { _gr_checks.inner(), STR("log"     ) },
                        ckBx_savExportSond  { _gr_checks.inner(), STR("Exp. probes" ) },
                        ckBx_savExportTarg  { _gr_checks.inner(), STR("Exp. targets") },
                        ckBx_loadNNParam    { _gr_dir.inner()   , STR("load at start") },
                        ckBx_savNNParam     { _gr_checks.inner(), STR("save NNparam") }/*,*/
                        ;

    ParamGUIBind::BindGroup  _setup;

    void  SetDefLayout   () override ;
    virtual void  AsignWidgetToFields () final  override;
    void  MakeResponive();
    void  SaveProj();
    void  setAsDefProject();
    void  RestDefPr	 ( )	;	// Restore (USE) Deff  Proj File

public:     
    OpenSaveBox         _proj       { *this, STR("Project:") };

    SetupPage (ThDyNanaForm& tdForm);

    static FilePickBox& AddFastaFiltre(FilePickBox &fpb)
    {
        return fpb.add_filter(FastaFiltre( ));
    }
    static FilePickBox::filtres FastaFiltre( );


    void AddMenuItems(nana::menu& menu);
    void LoadProjectAndReg(nana::string file);
    void LoadProject(nana::string file);
};

#endif