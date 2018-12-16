import org.jacop.core.BooleanVar;
import org.jacop.core.Store;
import org.jacop.jasat.utils.structures.IntVec;
import org.jacop.satwrapper.SatWrapper;
import org.jacop.search.DepthFirstSearch;
import org.jacop.search.IndomainMin;
import org.jacop.search.Search;
import org.jacop.search.SelectChoicePoint;
import org.jacop.search.SimpleSelect;
import org.jacop.search.SmallestDomain;
import java.util.*;
import java.util.ArrayList;
import java.io.*;


public class SATPaganitzu {

	public static void main(String args[]){
		int nSnakes = Integer.parseInt(args[1]);
		String name = args[0];
		String linea;
		ArrayList <ArrayList<Character>> maze = new ArrayList();
		ArrayList <BooleanVar> [] columnasSnakes= new ArrayList[nSnakes];
		ArrayList <BooleanVar> [] filasSnakes= new ArrayList[nSnakes];
		ArrayList <BooleanVar> columnasAl = new ArrayList();
		ArrayList <BooleanVar> filasAl = new ArrayList();
		int nColumnas = 0;
		int nFilas = 0;

		// Abrimos el archivo
		// Guardamos en una matriz las posiciones libres del mazo

		try{
			File f = new File(name);
			FileReader fr = new FileReader(f);
			BufferedReader br = new BufferedReader(fr);
			linea =br.readLine();
			nColumnas = linea.length();
		while(linea!=null){
			ArrayList<Character> auxM = new ArrayList();
			if(linea.length()<nColumnas) nColumnas = linea.length();
			/*
			for(int i = 0;i<nSnakes; i++){
				ArrayList<Boolean> auxF = new ArrayList();
				filasSnakes[i].add(auxF);
			}*/
			for(int j = 0; j<linea.length();++j){
				if(linea.charAt(j)=='S'||linea.charAt(j)=='K'||linea.charAt(j)=='E'||linea.charAt(j)=='%'||linea.charAt(j)=='O') auxM.add(linea.charAt(j));
				else if(linea.charAt(j)==' ') auxM.add(' ');
				//else System.out.println("WRONG MAZE: bad caracters would appear in the output with a W"); auxM.add('W');
			}
			maze.add(auxM);
			nFilas++;
			linea =br.readLine();
		}
	}catch(Exception e){ e.printStackTrace();};






		Store store = new Store();
		SatWrapper satWrapper = new SatWrapper();
		store.impose(satWrapper);

		// Todas las variables: es necesario para el SimpleSelect

		BooleanVar[] allVariables = new BooleanVar[(nSnakes+1)*(nFilas+nColumnas)];

		// Creación de las variables
		for(int i = 0; i<nSnakes;++i){
			ArrayList <BooleanVar> auxSc = new ArrayList();
			for(int k = 0; k<nColumnas;++k){
					BooleanVar auxBv = new BooleanVar(store, "Hay un "+i+" serpiente en la columna "+k);
					auxSc.add(auxBv);
					satWrapper.register(auxBv);
					allVariables[(i)*(nColumnas+nFilas)+k] = auxBv;
				}
				columnasSnakes[i] = auxSc;
				ArrayList <BooleanVar> auxSf = new ArrayList();
			for(int j = 0; j<nFilas;++j){
					BooleanVar auxBv = new BooleanVar(store, "Hay un "+i+" serpiente en la fila "+j);
					auxSf.add(auxBv);
					satWrapper.register(auxBv);
					allVariables[(i)*(nColumnas+nFilas)+nColumnas+j] = auxBv;

			}
			filasSnakes[i] = auxSf;

		}

		for(int i = 0;  i<nColumnas; ++i){
			BooleanVar auxBv = new BooleanVar(store, "Hay un Al en la columna "+i);
			columnasAl.add(auxBv);
			satWrapper.register(auxBv);
			allVariables[(nSnakes)*(nColumnas+nFilas)+i] = auxBv;
		}
		for(int i = 0; i<nFilas; ++i){
			BooleanVar auxBv = new BooleanVar(store, "Hay un Al en la fila "+i);
			filasAl.add(auxBv);
			satWrapper.register(auxBv);
			allVariables[(nSnakes)*(nColumnas+nFilas)+nColumnas+i] = auxBv;
		}



		// Obtenemos los literales no negados de las variables
		int [][] literalColumnas= new int[nSnakes+1][nColumnas];
		int [][] literalFilas= new int[nSnakes+1][nFilas];
		for(int s = 0; s<nSnakes+1;++s){
			for(int i = 0; i < nFilas; ++i){
				if(s==nSnakes) literalFilas[s][i] = satWrapper.cpVarToBoolVar(filasAl.get(i), 1, true);
				else 	literalFilas[s][i] = satWrapper.cpVarToBoolVar(filasSnakes[s].get(i), 1, true);

			}
			for(int i = 0;i < nColumnas; ++i){
				if(s==nSnakes) literalColumnas[s][i] = satWrapper.cpVarToBoolVar(columnasAl.get(i), 1, true);
				else 	literalColumnas[s][i] = satWrapper.cpVarToBoolVar(columnasSnakes[s].get(i), 1, true);
			}
		}




		// Imponemos las restricciones
		for(int s = 0; s<nSnakes+1; ++s){
			for(int i = 0; i<nFilas; ++i){

				//two snakes cannot be placed in the same row (also Al cannot be in the same row)
				for(int e = s+1; e<nSnakes+1;e++){
					addClause(satWrapper, -literalFilas[s][i], -literalFilas[e][i]);

				}

				for(int j = 0; j<nColumnas;++j){
						//Verificamos que haya hueco en el mapa
						if(maze.get(i).get(j)!=' ') addClause(satWrapper, -literalFilas[s][i], -literalColumnas[s][j]);
						if(s<nSnakes) 	addClause(satWrapper, -literalColumnas[nSnakes][j], -literalColumnas[s][j]);
				}
			}
			for(int j = 0; j<nColumnas;++j){
					//Verificamos que no haya snakes en la misma columna que Al
					if(s<nSnakes) 	addClause(satWrapper, -literalColumnas[nSnakes][j], -literalColumnas[s][j]);
			}
		}


		 for(int s = 0; s<nSnakes+1; ++s){
			 //IMPONER QUE SI UNA ROW SE ACTIVA NO SE PUEDE ACTIVAR OTRA DEL MISMO SNAKE/AL
			 for(int i = 0; i<nFilas; ++i){
				 for(int j = i+1; j<nFilas; ++j) addClause(satWrapper, -literalFilas[s][i], -literalFilas[s][j]);
			 }
			 //IMPONER QUE SI UNA COLUMN SE ACTIVA NO SE PUEDE ACTIVAR OTRA DEL MISMO SNAKE/AL
			 for(int i = 0; i<nColumnas; ++i){
				 for(int j = i+1; j<nColumnas; ++j)	addClause(satWrapper, -literalColumnas[s][i], -literalColumnas[s][j]);
			 }

			 //IMPONER QUE SI UNA ROW SE ACTIVA UNA COLUMNA
			 for(int i = 0; i<nFilas; ++i){
				 IntVec clause = new IntVec(satWrapper.pool);
				 clause.add(-literalFilas[s][i]);
				 for(int j = 0; j<nColumnas; ++j){
			 		clause.add(literalColumnas[s][j]);
				 }
				 satWrapper.addModelClause(clause.toArray());
			 }

			 //IMPONER QUE SI UNA COLUMN SE ACTIVA UNA ROW
			 for(int i = 0; i<nColumnas; ++i){
				 IntVec clause = new IntVec(satWrapper.pool);
				 clause.add(-literalColumnas[s][i]);
				 for(int j = 0; j<nFilas; ++j){
			 		clause.add(literalFilas[s][j]);
				 }
				 satWrapper.addModelClause(clause.toArray());
			 }

			 //IMPONER QUE AL MENOS SE ACTIVE UNA ROW Y UNA COLUMN DE CADA SNAKE Y AL
			 IntVec clauseC = new IntVec(satWrapper.pool);
			 for(int i = 0; i<nColumnas; ++i) clauseC.add(literalColumnas[s][i]);
			 satWrapper.addModelClause(clauseC.toArray());

			 IntVec clauseF = new IntVec(satWrapper.pool);
			 for(int i = 0; i<nFilas; ++i) clauseF.add(literalFilas[s][i]);
			 satWrapper.addModelClause(clauseF.toArray());

		 }






	    Search<BooleanVar> search = new DepthFirstSearch<BooleanVar>();
		SelectChoicePoint<BooleanVar> select = new SimpleSelect<BooleanVar>(allVariables,
							 new SmallestDomain<BooleanVar>(), new IndomainMin<BooleanVar>());
		Boolean result = search.labeling(store, select);

		if (result) {
			System.out.println("Solution: ");

			for(int s = 0; s<nSnakes; ++s){
				for(int i = 0; i<nFilas; ++i){
					if(filasSnakes[s].get(i).dom().value()==1){
						for(int j = 0; j<nColumnas; ++j){
							if(columnasSnakes[s].get(j).dom().value()==1){
								 maze.get(i).set(j,'S');
							}
						}
					}
				}
				System.out.println();

				}
				for(int i = 0; i<nFilas; ++i){
					if(filasAl.get(i).dom().value()==1){
						for(int j = 0; j<nColumnas; ++j){
							if(columnasAl.get(j).dom().value()==1){
								maze.get(i).set(j,'A');
							}
						}
					}
				}

			}else System.out.println("NO RESULT FOUND");

		System.out.println();
		finalOutput(maze, name);
	}



	public static void addClause(SatWrapper satWrapper, int literal1, int literal2){
		IntVec clause = new IntVec(satWrapper.pool);
		clause.add(literal1);
		clause.add(literal2);
		satWrapper.addModelClause(clause.toArray());
	}

	public static void finalOutput(ArrayList <ArrayList<Character>> maze, String name){
		try {
			Writer writer = new BufferedWriter(new OutputStreamWriter(
              new FileOutputStream(name+".output"), "utf-8"));
							String auxSt = new String();

				for(int i = 0; i<maze.size(); ++i){

					for(int j = 0; j<maze.get(i).size(); ++j) auxSt += maze.get(i).get(j);
					auxSt += "\n";
				}
				writer.write(auxSt);
				writer.close();
		} catch(Exception e){ e.printStackTrace();};
	}

}
