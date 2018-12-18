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

		/*
		The input file is opened
		We store the maze on a data structure
		*/

		try{
			File f = new File(name);
			FileReader fr = new FileReader(f);
			BufferedReader br = new BufferedReader(fr);
			linea =br.readLine();
			nColumnas = linea.length();
		while(linea!=null){
			ArrayList<Character> auxM = new ArrayList();

			//We take the maximum rectangule contained in the map
			if(linea.length()<nColumnas) nColumnas = linea.length();
			for(int j = 0; j<linea.length();++j){

				//Wrong input Characters should be ignore and they won't appear in the output
				if(linea.charAt(j)=='K'||linea.charAt(j)=='E'||linea.charAt(j)=='%'||linea.charAt(j)=='O'||linea.charAt(j)==' ') auxM.add(linea.charAt(j));
				else auxM.add(' ');
			}
			maze.add(auxM);
			nFilas++;
			linea =br.readLine();
		}
	}catch(Exception e){ e.printStackTrace();};


		Store store = new Store();
		SatWrapper satWrapper = new SatWrapper();
		store.impose(satWrapper);

		//Storing all the variables
		BooleanVar[] allVariables = new BooleanVar[(nSnakes+1)*(nFilas+nColumnas)];

		// Creation of variables for snakes
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

		// Creation of variables for Al
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



		// Obtaining non negated literals
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

		// Imposing Constraints
		for(int s = 0; s<nSnakes+1; ++s){
			for(int i = 0; i<nFilas; ++i){

				//two snakes cannot be placed in the same row (also Al cannot be in the same row)
				for(int e = s+1; e<nSnakes+1;e++){
					addClause(satWrapper, -literalFilas[s][i], -literalFilas[e][i]);

				}

				for(int j = 0; j<nColumnas;++j){

						//Only if there is a free space can be located
						if(maze.get(i).get(j)!=' ') addClause(satWrapper, -literalFilas[s][i], -literalColumnas[s][j]);
				}

				//Only one row can be activated on the same snake/Al
				for(int j = i+1; j<nFilas; ++j) addClause(satWrapper, -literalFilas[s][i], -literalFilas[s][j]);
			}

			for(int i = 0; i<nColumnas; ++i){

				//A snake cannot be in the same place as Al
				if(s<nSnakes) 	addClause(satWrapper, -literalColumnas[nSnakes][i], -literalColumnas[s][i]);

				//Only one column could be activated on the same snake/Al
				for(int j = i+1; j<nColumnas; ++j)	addClause(satWrapper, -literalColumnas[s][i], -literalColumnas[s][j]);
			}

			//All the snakes/Al must be placed somewhere on the maze
			//At least one column has to be activated
			IntVec clauseC = new IntVec(satWrapper.pool);
			for(int i = 0; i<nColumnas; ++i) clauseC.add(literalColumnas[s][i]);
			satWrapper.addModelClause(clauseC.toArray());
			//At least one row has to be activated
			IntVec clauseF = new IntVec(satWrapper.pool);
			for(int i = 0; i<nFilas; ++i) clauseF.add(literalFilas[s][i]);
			satWrapper.addModelClause(clauseF.toArray());

		}

	  Search<BooleanVar> search = new DepthFirstSearch<BooleanVar>();
		SelectChoicePoint<BooleanVar> select = new SimpleSelect<BooleanVar>(allVariables, new SmallestDomain<BooleanVar>(), new IndomainMin<BooleanVar>());
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

			}else System.out.println("NO RESULT FOUND AND NO OUTPUT GENERATED");

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

		//creation of the output file
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
