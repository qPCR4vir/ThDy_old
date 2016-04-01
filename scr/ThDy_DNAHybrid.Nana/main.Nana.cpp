/**
* Copyright (C) 2009-2016, Ariel Vina-Rodriguez ( ariel.rodriguez@fli.bund.de , arielvina@yahoo.es )
*
* @autor Ariel Vina-Rodriguez (qPCR4vir)
* 2012-2016
*
* @file  ThDySec\scr\ThDy_DNAHybrid.Nana\main.Nana.cpp
*
* @brief 
*/

#include "ThDy_DNAHybrid.Nana\main.Nana.h"

//if you want to keep the Windows subsystem you can just hint at what your entry point is, 
//because you haven't defined ___tmainCRTStartup. You can do this by adding the following 
//to Properties -> Linker -> Command line:
//
//    /ENTRY:"mainCRTStartup"
//
//This way you get rid of the console window.


int main(int argc, char *argv[]) 
{

    //std::ifstream in("in.txt");
    //std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    //std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!

    //std::ofstream out("out.txt");
    //std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    //std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    //std::string word;
    //std::cin >> word;           //input from the file in.txt
    //std::cout << word << "  ";  //output to the file out.txt

    //f(); //call function


    //std::cin.rdbuf(cinbuf);   //reset to standard input again

  //nana::color::current_schema[nana::color::schema::list_header_border]=nana::color::Red;
  //nana::color::current_schema[nana::color::schema::list_header_bg]=nana::color::Yellow;    // 0xF1F2F4 
  //nana::color::current_schema[nana::color::schema::list_header_highlighted_bg]=nana::color::Rose;    // 0xFFFFFF 
  //nana::color::current_schema[nana::color::schema::list_header_pressed_bg]=nana::color::AliceBlue;  
  //nana::color::current_schema[nana::color::schema::list_header_grabed_bg]=nana::color::Ash_Gray;    // 0x8BD6F6 
  //nana::color::current_schema[nana::color::schema::list_header_floated_bg]=nana::color::Aztech_Purple;	   // 0xBABBBC 
  //nana::widget_colors::background ;   ::current_schema[nana::color::schema::list_header_floated_bg]=nana::color::Aztech_Purple;	   // 0xBABBBC 
  //nana::form::scheme_type().background = nana::colors::light_sky_blue;
  try	
  {
    using namespace ParamGUIBind;


    IParBind::SetDef(PriorizeDefault::Parametr );
    ThDyNanaForm tdForm(  argc,  argv);
    //tdForm.ReCollocate();
	tdForm.show();
	nana::exec();
	return 0;
  }
    catch (std::exception& e)
        {
            std::cerr<< std::endl<< e.what();
            throw ;
        } 
    catch (...)
        {
            std::cerr<< std::endl<< "exeption !!";
            throw ;
        }

    
    //std::cout.rdbuf(coutbuf); //reset to standard output again

} 




    ThDyNanaForm::ThDyNanaForm (int argc, char *argv[])
                  :nana::form (nana::rectangle( nana::point(50,5), nana::size(1000,650) )),
                   EditableForm    (nullptr, *this,  "ThDy DNA Hybrid"  , "ThDy.lay.txt")  
   {
        //nana::API::zoom_window(*this, true);
        //nana::pixel_rgb_t bk;
        //bk.u.color = background ();
        //bk.u.element.blue =0; 
        //bgcolor (nana::color_rgb( 0xEEEEEE));  ///\todo: use codigo
        //foreground(1);
       //this->scheme().background(col)
        //this->b
        add_page( setup_    );// setup_.ReCollocate(); // 0 
        add_page( mExpl_    ); //mExpl_.ReCollocate();// 1
        add_page( findSond_ );// findSond_.ReCollocate();// 2
        add_page( mPCR_     ); //mPCR_.ReCollocate();// 3
        add_page( uArr_     );// uArr_.ReCollocate();// 4
        add_page( tmCalc_   );// tmCalc_.ReCollocate();// 5

        tabbar_.activated (1);

        setup_._proj.FileNameOnly(nana::charset ( ProjetFile()  ));
        try{ 
			    if ( argc > 1 )
				    setup_._proj.FileNameOpen(nana::charset( argv[1]) )   ;
			    else
				    load() ;						
		    }
    	catch ( std::exception& e )      // Por ejemplo cuando no existe Def Project: 1ra vez que se usa el prog.
		{   
            (nana::msgbox(*this, "Error during initial project load !\n\t", nana::msgbox::button_t::ok)
                             .icon(nana::msgbox::icon_information )
                            << e.what()    << "\n\n A new Default Project will be created. "
                          ).show (  ) ;
		    save_defPr() ; 					                
        }

		//this->comBoxTAMeth->SelectedIndex  = SMStLucia;    
        try{ 
		        _cp.Actualize_All_NNp();
                LoadSequences();
		    }
    	catch ( std::exception& e )      //  
		{   
            (nana::msgbox(*this, "Error during sequence or NN parametr load !\n\t", nana::msgbox::button_t::ok)
                             .icon(nana::msgbox::icon_information )
                            << e.what()     
                          ).show (  ) ;
		    save_defPr() ; 					                
        }
		this->_uArr._probesMS->CreateNonDegSetRec();
		this->_TmCal._probesMS->CreateNonDegSetRec();
		this->_mPCR._probesMS->CreateNonDegSetRec();
        mExpl_.InitTree();

        InitMyLayout();

        setup_.AddMenuItems (_menuBar.push_back("P&roject"));     // 0
        mExpl_.AddMenuItems (_menuBar.push_back("&Sequences"));   // 1 
        AddMenuProgram();                                         // 2
        
        SelectClickableWidget( _menuBar);

		//nana::fn_group<void(tabbar&, value_type&)> active;

		tabbar_.events().activated( [this]()// const tabbar & t, const tabbar::value_type& tab)
		{
			bool enable	= tabbar_.activated( )==1;
			auto &m		= _menuBar. at(1);
			auto sz		= m.size();

			for(int i=0; i< sz; ++i)
				m.enabled(i,enable);
		});


   }

    //~ThDyNanaForm();
    void     ThDyNanaForm::SetDefLayout   () 
    {
        _DefLayout=
	        "vertical      gap=2                   	\n\t"
	        "	 vertical      gap=2                   		\n\t"
	        "		        <weight=25>                   		\n\t"
	        "		        <PagesTag    weight=23 >      		\n\t"
	        "		        <Pages       min=255   >      		\n\t"
	        "		        < <weight=30><TargetsOptions min=640 max=1000><weight=10> weight=23>      		\n\t"
	        "		        <weight=1 >                   		\n\t"
	        "		        < weight=21 <><Firma weight=280> <weight=3 > >                   		\n\t"
	        "		 		\n\t"
	        "		\n\t"
            ;

        numUpDwMaxTgId .ResetLayout (60,40,30 );  
        numUpDw_TgBeg  .ResetLayout (35,40,30 );  
        numUpDw_TgEnd  .ResetLayout (35,40,30 );  
        numUpDw_SLenMin.ResetLayout (60,40,30 );   
        numUpDw_SLenMax.ResetLayout (60,70,30 );  
    }
    void     ThDyNanaForm::AsignWidgetToFields() 
    {
        using ParamGUIBind::link;

        _commPP  << link( _cp.MaxTgId                 ,       numUpDwMaxTgId  )
                 << link( _cp.SecLim         , numUpDw_TgBeg,  numUpDw_TgEnd  )
                 << link( _cp.SecLenLim   ,  numUpDw_SLenMin, numUpDw_SLenMax )
            ;
 
	    _place.field("PagesTag")        << tabbar_  ;
	    _place.field("TargetsOptions" ) << numUpDwMaxTgId<<   numUpDw_TgBeg << numUpDw_TgEnd << numUpDw_SLenMin << numUpDw_SLenMax;
	    _place.field("Firma"  )         << " ArielVina.Rodriguez@fli.bund.de"
                                ;
    }                                        
    void     ThDyNanaForm::add_page(widget& w)
    {   
        //nana::pixel_rgb_t bk;
        //bk.u.color = background ();
        //bk.u.element.blue =0; 
        //w.background (1);
        tabbar_.push_back (                    w.caption());
        tabbar_.attach    (tabbar_.length()-1, w          );
	    _place.field("Pages"   ).fasten( w)  ;
    }         

