# proyectoAlgoritmos
Proyecto de Algoritmos y Estructura de datos (2018) - K1091

# Parte 1:
### Presentación
El objetivo de esta primera parte es poner en práctica el concepto de Estructuras de control de flujo                                  
de un programa (secuencia, análisis de caso y repetición), como asimismo trabajar con tipos de datos                              
simples y estructurados (arrays), con expresiones lógicas, asignación interna y externa. También                      
usaremos flujos conocidos como “Archivos” y patrones algorítmicos de ordenamiento.

## Ejercicio 1
El sorteo de la fase de grupos del Mundial de Fútbol se inicia agrupando a los 32 equipos clasificados                                    
en 4 bolilleros de 8 equipos cada uno, ordenados de acuerdo al FIFA ranking. Los mejores                              
pertenecen al bolillero 1 (salvo Rusia que es el anfitrión), los equipos que están por debajo en el                                  
ranking se asignan al bolillero 4; conforme a este ranking se forma la siguiente Tabla:

+-------------+--------------+-------------+----------------+
| BOLILLERO 1 |  BOLILLERO 2 | BOLILLERO 3 |  BOLILLERO 4   |
+-------------+--------------+-------------+----------------+
| Rusia       | España       | Dinamarca   | Serbia         |
| Alemania    | Perú         | Islandia    | Nigeria        |
| Brasil      | Suiza        | Costa Rica  | Australia      |
| Portugal    | Inglaterra   | Suecia      | Japón          |
| Argentina   | Colombia     | Túnez       | Marruecos      |
| Bélgica     | México       | Egipto      | Panamá         |
| Polonia     | Uruguay      | Senegal     | Corea del Sur  |
| Francia     | Croacia      | Irán        | Arabia Saudita |
+-------------+--------------+-------------+----------------+


Se solicita generar un archivo binario con los equipos clasificados al mundial conforme a la tabla                              
detallada precedentemente. Cada registro debe contener como mínimo los siguientes campos:                    
número de bolillero, nombre de equipo y confederación a la que pertenece el equipo; se debe                              
generar ordenado por número de bolillero.
Las Confederaciones continentales aglutinan a un conjunto de países, son seis (6) y se detallan a                              
continuación:
● AFC: Copa Asiática (países de Asia, excepto Israel , Kazajistán y Turquía ; más Australia, Islas                              
Marianas del Norte y Guam). 
● CAF: Copa Africana de Naciones (países de África).
● Concacaf: Copa Concacaf (América del Norte, América Central, las islas del Caribe, Guyana,                        
Surinam y Guayana Francesa).
● Conmebol: Copa América (países de América del Sur, excepto Guyana, Surinam y Guayana                        
Francesa, más dos invitados).
● OFC: Copa de las Naciones (países de Oceanía excepto Australia, Islas Marianas del Norte,                          
Guam y Palaos).
● UEFA: Eurocopa (países de Europa, Israel , Kazajistán y Turquía).
Ejercicio 2
Diseñar e implementar el algoritmo que permita obtener los 8 Grupos del Mundial (identificados con                            
las letras de la A a la H). El sistema debe dar la opción al usuario por pantalla para que pueda elegir                                          
“Sortear Fase de Grupos”, una vez seleccionada esta opción el sistema responde realizando el sorteo                            
e informando por pantalla la conformación de los 8 grupos del mundial. Asimismo, se debe generar                              
un archivo por cada grupo en donde cada registro debe tener los siguientes campos: número de                              
bolillero, nombre de equipo y confederación a la que pertenece el equipo. El algoritmo, para llevar a                                
cabo esta tarea, debe cumplir con las siguientes reglas y pasos:
Regla 1: Cada grupo está formado por un equipo de cada bolillero.
Regla 2: En cada grupo no pueden existir equipos de la misma Confederación (salvo UEFA Eurocopa                              
que puede tener un máximo de 2); en cambio los equipos Sudamericanos del bolillero 2, por                              
ejemplo, no podrán integrar los grupos de Brasil ni de Argentina.
Pasos del algoritmo:
Paso 1: Se sortea el bolillero 1. Rusia va al grupo A por ser el anfitrión, los demás irán a los 7 grupos                                            
restantes en orden de salida (de la B a la H).
Paso 2: Se sortea el bolillero 2. Si un equipo Sudamericano sale sorteado en el grupo de Brasil o                                    
Argentina, se repite el Paso 2.
Paso 3: Se sortea el bolillero 3. Si un equipo sale sorteado en un grupo que ya tenga un miembro de                                        
la misma Confederación (o 2 en el caso Europeo), se repite el Paso 3.
Paso 4:  Se sortea  el bolillero 4. Se repite el proceso dado para el Paso 3.

## Ejercicio 3
Procesar los archivos generados en el punto anterior (Ejercicio 2) para cargar un Array donde cada                              
elemento tiene la siguiente estructura:
Nombre de Equipo
Letra de Grupo
Confederación
Se solicita mostrar por pantalla el contenido del array ordenado por los siguientes criterios:
● Número de Grupo.
● Nombre de equipo.
● Confederación.
● Número de Grupo y confederación.
● Número de Grupo, confederación y nombre de equipo.