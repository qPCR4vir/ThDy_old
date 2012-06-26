Original Dynnamic Program recursion for Alignment by Mith-Waterman

				S(i-1, j-i) + w(i,j)		, Macth / MitsMAtch   PM / MM
S(i,j)= max		S(i-1, j  ) + w(i,-)		, gap en j, b, target
				S(i  , j-1) + w(-,i)		, gap en i, a, sonde


osea que viene en diag, de la cell de al lado o de la de arriba, mas el propio matcheo o gap de la cell que se calcula,
osea en cada cell se guarda el alineamiento que viniendo de las vecinas se hace max con el nuevo matcheo.
Esto es cuando el nuevo matcheo se puede calcular usando solo las bases o gaps correspondiente a la nueva cell/movimiento
osea al ultimo pareo.

Pero en el NN model depende de los ultimos dos pareos.
Si analizo en una celda el pareo actual , eso afectaria tambien al siguiente, ya que cada pareo afecta 2 posiciones, cada base 
interacua con la compl, pero tambien con la ant y post. 
Por eso en la celda actual conservo no el resultado del pareo actual , sino el de las dos pares anterioes.
Aqui tenemos no w(i,j) , si no w(i-1, i , j-1, j) , el pareo no es de las ultimas dos base , si no de las ultimos dos pares

Asi que usaremos no w(i-1, i , j-1, j) para calcular la celda actual, si no w(i-2, i-1 , j-2, j-1) 

basado en los dos pares anteriores y que ya no afecta las celdas siguientes.
De manera que un nuevo paso cambia el recorrido optimo no solo hasta la ultima celda, sino tambien hasta la penultima.
Asi que debo maximizar (encontrar el max) no solo el ultimo paso , si no tambien el anterior, sumando todo para encontar el nuevo max. 
Por eso en lugar de depender de las tres celdas anteriores, depende de esas tres mas (por,x) las tres de las 
que dependian cada una de ellas, que serian 9 celdas anteriores. 
De aqui se puede hacer una pequena disminucion, ya que venir de la izq y a continuacion bajar, o viceversa, solo
significa anadir un gap al frente de otro, lo cual nunca mejoraria nuestro alineaminto. Asi que de las celdas de arriba y de la 
izq solo tomo dos precursores.


La G, energia que se nececita para formar la estructura (el hibrido) a una determinada temperatura : la temperatura a la que 
transcurre la hibridacion, la temperatura de aneling - Ta. Asumimos: -G = H - Ta*S, 
o G = Ta*S - H
A altas temperaturas el factor desorden, entropia, predomina y se necesitaria energia externa para mantener las moleculas unidas 
A bajas temperaturas al contrario la reaccion de hibridacion entrega energia y ocurre espontaneamente.

La Ta a la cual la mitad de las moleculas limitantes (a menor concentracion) 
estan hibridadas se llama Temperatura de melting Tm y aqui no hay energia asociada, la G=0. 
Interesante saber otras proporciones de participacion de las molec en la
hibridacion : la forma de la curva con "centro" en Tm. Permititia mucho mejor determinar la eficiencia de una hibiridacion, y por
tanto de un PCR:  seria como calcular la concentracion efectiva de la sonda o del template a la Ta selecionada.
Asumimos: -G = H - Ta*S, de donde 0= H - Tm*S, o Tm=H/S

El modelo NN de la termodinamica de la hibridacon dfe moleculas de ac.nucleicos afirma que las contibuciones decicivas son solo 
entre las dos bases que hibridan, complementarias o no, mas la relacionada con las vecinas en la proia cadena, osea que la
H y S se pueden calcular como la suma para cada pociosion la hibridacion i de H(   ) 



	s2		s1			jb		ia						8	4	2	1					
	jb	ia	jb	ia		s2	s1	s2	s1				0	0	0	0	0					
s01	0	1	0	1	5	0	0	1	1	3			1	0	0	0	1		15	14	13	12
s02	1	1	0	1	13	1	0	1	1	11			2	0	0	1	0		11	10	9	8
s03	0	1	1	1	7	0	1	1	1	7			3	0	0	1	1		7	6	5	4
s04	1	1	1	1	15	1	1	1	1	15			4	0	1	0	0		3	2	1	0
s05	1	0	1	1	11	1	1	0	1	13			5	0	1	0	1					
s06	1	1	1	0	14	1	1	1	0	14			6	0	1	1	0					
s07	1	0	1	0	10	1	1	0	0	12			7	0	1	1	1					
