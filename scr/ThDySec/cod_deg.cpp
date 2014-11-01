//#include "stdafx.h"
//#pragma unmanaged
#include "ThDySec/cod_deg.h"
namespace DegCod
{
 
 Base		is_base		[UCHAR_MAX],		///< <> 0  si base. =base, pero para U, =T 
			is_degbase	[UCHAR_MAX],		///< <> 0  si letra valida (cualquiera del cod deg,may o minuscula
											///< + insercion '-').=base, pero para U, =T 
			c_degbase	[UCHAR_MAX],		///<  devuelve base complementaria, tambien para codigo deg. 
											///< El resto no lo modifica.
			grad_deg	[UCHAR_MAX];		///< grado de degeneracion: 0- no permitido, 1-base, 
											///<						  2-Y,R,K....,     4-N.
 Code		is_GC		[UCHAR_MAX],		///< 1-G or C, 0-lo demas.
			bk2nu		[UCHAR_MAX],		///< codigo corto de Kadelari
			bk2c_nu		[UCHAR_MAX],		///< codigo corto de Kadelari complementario a la base
			bkn2c_nu	[n_basek],			///< codigo corto de Kadelari complementario
			ba2nu		[UCHAR_MAX],		///< codigo corto
			ba2c_nu		[UCHAR_MAX],		///< codigo corto complementario a la base
			ban2c_nu	[n_ba],				///< codigo corto complementario
			dbn2c_nu	[n_dgba],			///< cod numerico complementario a la base deg
			db2c_nu		[UCHAR_MAX],		///< cod numerico complementario a la base deg
			db2nu		[UCHAR_MAX];		///< dada la letra devuelve el cod numerico; lo contrario 
											///< de degcod[]; 0- no validos, pero tambien el '-'.
											///< si las bases a y b pueden match se comprueba: 
											///< if (base2bin[a] & base2bin[b]) ...   : ejemplo - si match 
											///< R y G, R y K, ...
											///< (base2bin[a] & base2bin[b]) da el cod de la min letra comun:
											///< R y G = G, R y K = G
											///< para calcular consenso: degcod[base2bin[a] | base2bin[b])]
 Base		dg2ba		[n_dgba][n_ba],		///< para generar todas las variantes de una base deg
			dg2ban		[n_dgba][n_ba],
			dg2bkn		[n_dgba][n_ba];

 const		Base	*c_base      =c_degbase,
					*c_basek     =c_degbase	;


CInit_Cod_Deg::CInit_Cod_Deg()	
{			
	        for (Base b=0; b<UCHAR_MAX; b++)	
			{	is_base		[b]	= 
				is_degbase	[b] = 
				is_GC		[b] =
				grad_deg	[b] = 0 ;  
				bk2nu		[b]	=
				db2nu		[b]	=
				ba2nu		[b]	= 0 ;   // por ejemplo '.' or '-'

				c_degbase	[b]	= b ;	// solo cambia las bases !!!!
			}


			for (Base b=0; b<n_ba  ; b++)		
			{	is_base	[ nu2ba[b] ]= is_base	[ tolower(nu2ba[b]) ] =	nu2ba[b] ;
				ba2nu	[ nu2ba[b] ]= ba2nu		[ tolower(nu2ba[b]) ] =	b ;
			}

			for (Base b=0; b<n_basek  ; b++)		
				bk2nu[ basek[b] ]= bk2nu[ tolower(basek[b]) ] = b ;	
				

			for (Base b=0; b<n_dgba; b++)		
			{	Base db=nu2dgba[b], ldg=Base(tolower(db));

				is_degbase	[ db ]	= is_degbase[ ldg ]	= nu2dgba[b] ;
				c_degbase	[ db ]	= c_degbase	[ ldg ]	= nu2c_dgba[b]   ; // '.' y '$' y tambien '-' quedan igual
				db2nu		[ db ]	= db2nu		[ ldg ]	= b   ;
			}
		
			
			for (Base bd=0; bd<n_dgba; bd++)
			for (Base b =0; b <n_ba  ; b++)
			{	grad_deg	[         nu2dgba[bd]  ] += (0!=(bd &  db2nu	[nu2ba[b]]));	
				grad_deg	[ tolower(nu2dgba[bd]) ] += (0!=(bd &  db2nu	[nu2ba[b]]));	
			}

			is_GC['G']=
			is_GC['C']=1 ;

			is_base		[ 'U' ]		= is_base		[ 'u' ]		=			// repetido ???
			is_base		[ 'U' ]		= is_base		[ 'u' ]		= 
			is_degbase	[ 'U' ]		= is_degbase	[ 'u' ]		='T' ;

			c_degbase	[ 'U' ]		= c_degbase		[ 'u' ]		='A' ;
			grad_deg	[ 'U' ]		= grad_deg		[ 'u' ]		= 1  ;
			bk2nu		[ 'U' ]		= bk2nu			[ 'u' ]		= bk2nu [ 'A' ] ;
			db2nu		[ 'U' ]		= db2nu			[ 'u' ]		= db2nu [ 'A' ] ;
			ba2nu		[ 'U' ]		= ba2nu			[ 'u' ]		= ba2nu [ 'A' ] ;

			for (Base b=0; b<UCHAR_MAX; b++)	
			{	bk2c_nu		[b]	= bk2nu		[ c_degbase [b] ] ;			
				ba2c_nu		[b]	= ba2nu		[ c_degbase [b] ] ;
				db2c_nu		[b]	= db2nu		[ c_degbase [b] ] ;
			}

			for (Base b=0; b<n_ba     ; b++)		
				ban2c_nu[ b ]= ba2nu[ nu2c_ba [b] ] ;

			for (Base b=0; b<n_dgba   ; b++)		
				dbn2c_nu[ b ]= db2nu[ nu2c_dgba [b] ] ;

			for (Base b=0; b<n_basek  ; b++)		
				bkn2c_nu[ b ]= bk2nu[ basek_c[b] ]  ;	


			for (Base b=0; b<n_dgba   ; b++)			// para generar todas las variantes de una base deg
			for (Base c=0; c<n_ba     ; c++)			// default en 0
			{	dg2ba [b][c]= 
				dg2ban[b][c]=
				dg2bkn[b][c]= 0	;
			}
			for (Base b=0	  ; b<n_dgba   ; b++)		// recorre las bas deg
			for (Base c=0, d=0; c<n_ba     ; c++)		// recorre las bas, cod corto 
			{	Base e= b &  (db2nu [nu2ba [ c ]]);			
				if (e)									// cod deg (largo) del caracter intercepcion
				{	dg2ban[b][d  ]=				c	      ; // cod corto del caracter intercepcion
					dg2ba [b][d  ]= nu2ba [		c		] ; // caracter intercepcion
					dg2bkn[b][d++]= bk2nu [ nu2ba [ c ] ] ; // cod K del caracter intercepcion
				}
			}
// se usa asi: 
			//for (Base b=0; b <  n_dgba				; b++)			// recorre las bas deg, aqui solo para
			//														// mostrar todas las variante

			//for (Base c=0; c <= grad_deg[ nu2dgba[b] ]  ; c++)	// recorre las bas no deg de la base deg b
			//{		dg2ban[b][c]  ;    					// cod corto de cada caracter a generar
			//		dg2ba [b][c]  ;						// caracter a generar
	  //        db2nu[dg2ba [b][c]] ;						// cod deg (largo) de cada caracter a generar				
	  //    b & db2nu[dg2ba [b][c]] ;	
			//		dg2bkn[b][c]  ;						// cod K de cada caracter a generar
			//}
			
}


CInit_Cod_Deg Init_Cod_Deg;

long CountDegBases (const char *sec)
{	long cb=0 ;
	for (long i=0 ; sec[i] ; i++) if( is_degbase	[Base (sec[i])] ) cb++ ;
	return cb;
}
Base *Generate_DegSec( const char *sec, bool rev, bool compl, long l/*=0*/)  
{	if ( ! l ) l=CountDegBases (sec);
	Base *DegSec=new Base[l+1], b;			 DegSec[l]=0 ;
	if (rev && compl) { for(long p=l-1, i=0;  b=Base (sec[i]) ;  i++)	if (b=is_degbase[b]) DegSec[p--]=c_degbase[b]; return DegSec;	}
	if (rev         ) { for(long p=l-1, i=0;  b=Base (sec[i]) ;  i++)	if (b=is_degbase[b]) DegSec[p--]=          b ; return DegSec;	}
	if (       compl) { for(long p=0,   i=0;  b=Base (sec[i]) ;  i++)	if (b=is_degbase[b]) DegSec[p++]=c_degbase[b]; return DegSec;	}
                        for(long p=0,   i=0;  b=Base (sec[i]) ;  i++)	if (b=is_degbase[b]) DegSec[p++]=          b ; return DegSec;	
}


}