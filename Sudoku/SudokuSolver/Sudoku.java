import java.util.Scanner;
import java.util.Arrays;
public class Sudoku {
    public int[][] board = new int[9][9];
    public Sudoku() { //this is the default constructor, all cell are 0
        for (int rows = 0; rows < 9; rows++) {
            for (int columns = 0; columns < 9; columns++) { 
                board[rows][columns] = 0;
            }
        }
    }
    public Sudoku (int[][] board) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) { 
                this.board[i][j] = board[i][j];
            }
        } //the [][]array is getting intialized to the first constructor
    }
    public int[][] board() {
        //returns a copy of the current state of the board
        int[][] copyBoard = new int[9][9];
         for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) { 
                copyBoard[i][j] = board[i][j];
            }
        }
        return copyBoard;
    }
    public boolean[] candidates(int row, int column) {
        boolean[] b = new boolean[10]; 
        for (int j = 0; j < 10; j++) {
            b[0] = false;
            b[j] = true;
        }
        if ( board[row][column] != 0) {
            for ( int i = 0; i < 10; i++) { b[i] = false; }
            return b;
        }//for (int rows = 0; rows < 9; rows++ ) {
        for (int i = 0; i < 9; i ++) { // for a perticular cell of candidates we are only considering its column, 
            //row and box. not all rows and columns.
            for (int j = 1 ; j <= 9 ; j++) {
                if (board[row][i] == j) {
                    b[j] =  false;
                }
            }
        }
        for (int i = 0; i < 9; i++ ) {
            for (int j = 1; j <= 9; j++) {
                if (board[i][column] == j) {
                    b[j] =  false;
                }
            }
        }
        int x = 0;
        int x2 = 0;
        int y = 0;
        int y2 = 0;
        
        if(row < 3) {
            x = 0;
            x2 = 3;
        }
        else if(row < 6 && row > 2) {
            x = 3;
            x2 = 6;
        }
        else if(row < 9 && row > 5) {
            x = 6;
            x2 = 9;
        }
        if(column < 3) {
            y = 0;
            y2 = 3;
        }
        else if(column < 6 && column > 2) {
            y = 3;
            y2 = 6;
        }
        else if(column < 9 && column > 5) {
            y = 6;
            y2 = 9;
        }
        
        for (int i = x; i < x2; i++) {      
            for (int j = y; j < y2; j++) {
                for(int k = 0; k <= 9; k++) {
                    if (board[i][j] == k) {
                        b[k] = false;
                    }
                }
            }
        }
        
        return b;
    }
    public boolean nakedSingles() {
        //when possible candidates is ==1 then that number is the naked single 
        // then assign that number to the cell
        int count = 0;
        int n = 0;
        boolean lone  = false; // denotes a change in the program
        boolean [] numCandidates = new boolean[10];
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) { 
                count = 0;
                numCandidates = candidates(i, j);
                for (int k = 1; k < 10; k++) {
                    if (numCandidates[k] == true) {
                        count ++;
                        n = k;
                    }
                }
                if( count == 1) {
                    //System.out.print(count);
                    board[i][j] = n;
                    
                    lone = true;
                }
            }
        }
        return lone;  
    }
    public boolean hiddenSingles() { 
        boolean b = false;
        for (int i = 0; i < 9; i++ ) {
            for(int j = 0; j < 9; j++) {
                if(board[i][j] == 0) {
                    boolean[] posCan1 = candidates(i, j);
                    for (int k = 0; k < 9; k++ ) {
                        boolean[]  posCan2 = candidates(i, k);
                        if( k !=j) {
                            for (int l = 1; l < 10; l++) {
                                if ( posCan1[l] && posCan2[l]) {
                                    posCan1[l] = false;
                                }
                            }
                        }
                    }
                    int count = 0;
                    int num = 0;
                    for( int l  = 1; l < 10; l++) {
                        if(posCan1[l] == true) {
                            count++;
                            num = l;
                        }
                    }
                    if(count == 1) {
                        board[i][j] = num;
                        b = true;
                    }
                }
            }
        }
        for (int i = 0; i < 9; i++ ) {
            for(int j = 0; j < 9; j++) {
                if(board[i][j] == 0) {
                    boolean[] posCan1 = candidates(i, j);
                    for (int k = 0; k < 9; k++ ) {
                        boolean[]  posCan2 = candidates(k, j);
                        if ( k !=i) {
                            for (int l = 1; l < 10; l++) {
                                if ( posCan1[l] && posCan2[l]) {
                                    posCan1[l] = false;
                                }
                            }
                        }
                    }
                    int count = 0;
                    int num = 0;
                    for( int l  = 1; l < 10; l++) {
                        if(posCan1[l] == true) {
                            count++;
                            num = l;
                        }
                    }
                    if(count == 1) {
                        board[i][j] = num;
                        b = true;
                    }
                }
            }
        }
        return b;   
    }  
    public boolean isSolved() {
        //returns true if the board is in a solved state 
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) { 
                if (board[i][j] == 0) {
                    
                    return false;
                }
            }
        }
        return true;
    }
    public void solve() {
       while (!isSolved() && (nakedSingles() || hiddenSingles())) {
       }
        System.out.println("Solved!");
    }
    
    public static void main (String[] args) {
//        /*int[][] nakedSingles = {{759182463}, {816347529}, {234569718}, {967258341}, {148736295}, {325914687}, {582671934}, {493825176}, {671493852}};
//         int[][] given = {{000041000}, {060000200}, {000000000}, {320600000}, {000050040}, {700000000}, {000200300}, {480000000}, {501000000}};
//         int[][] row = {{000000000}, {816347529}, {234569718}, {967258341}, {148736295}, {325914687}, {582671934}, {493825176}, {671493852}};
//         int[][] column = {{059182463}, {016347529}, {034569718}, {067258341}, {048736295}, {025914687}, {082671934}, {093825176}, {071493852}};
//         int[][] box = {{000182463}, {000347529}, {000569718}, {967258341}, {148736295}, {325914687}, {582671934}, {493825176}, {671493852}};
//         int[][] hidenSingles = {{800739006}, {370465000}, {040182009}, {000600040}, {054300610}, {060500000}, {400853070}, {000271064}, {100940002}};
//         */
        //int[][] a = {{0, 2, 8, 0, 0, 7, 0, 0, 0}, {0, 1, 6, 0, 8, 3, 0, 7, 0}, {0, 0, 0, 0, 2, 0, 8, 5, 1}, {1, 3, 7, 2, 9, 0, 0, 0, 0}, {0, 0, 0, 7, 3, 0, 0, 0, 0}, {0, 0, 0, 0, 4, 6, 3, 0, 7}, {2, 9, 0, 0, 7, 0, 0, 0, 0}, {0, 0, 0, 8, 6, 0, 1, 4, 0}, {0, 0, 0, 3, 0, 0, 7, 0, 0}};
//        //String example = "800739006370465000040182009000600040054300610060500000400853070000271064100940002";
//        //int[][] a = {{8, 0, 0, 7, 3, 9, 0, 0, 6}, {3, 7, 0, 4, 6, 5, 0, 0, 0}, {0, 4, 0, 1, 8, 2, 0, 0, 9}, {0, 0, 0, 6, 0, 0, 0, 4, 0}, {0, 5, 4, 3, 0, 0, 6, 1, 0}, {0, 6, 0, 5, 0, 0, 0, 0, 0}, {4, 0, 0, 8, 5, 3, 0, 7, 0}, {0, 0, 0, 2, 7, 1, 0, 6, 4}, {1, 0, 0, 9, 4, 0, 0, 0, 2}};
  //      int[][] a = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {8, 1, 6, 3, 4, 7, 5, 2, 9}, {2, 3, 4, 5, 6, 9, 7, 1, 8}, {9, 6, 7, 2, 5, 8, 3, 4, 1}, {1, 4, 8, 7, 3, 6, 2, 9, 5}, {3, 2, 5, 9, 1, 4, 6, 8, 7}, {5, 8, 2, 6, 7, 1, 9, 3, 4}, {4, 9, 3, 8, 2, 5, 1, 7, 6}, {6, 7, 1, 4, 9, 3, 8, 5, 2}};
        int[][] a = {{8, 0, 0, 7, 3, 9, 0, 0, 6}, {3, 7, 0, 4, 6, 5, 0, 0, 0}, {0, 4, 0, 1, 8, 2, 0, 0, 9}, {0, 0, 0, 6, 0, 0, 0, 4, 0}, {0, 5, 4, 3, 0, 0, 6, 1, 0}, {0, 6, 0, 5, 0, 0, 0, 0, 0}, {4, 0, 0, 8, 5, 3, 0, 7, 0}, {0, 0, 0, 2, 7, 1, 0, 6, 4}, {1, 0, 0, 9, 4, 0, 0, 0, 2}};
        Sudoku s = new Sudoku(a);
        System.out.println("Sudoku:");
        System.out.println(Arrays.deepToString(s.board).replace("],", "],\n"));
        s.solve();
        System.out.println(Arrays.deepToString(s.board).replace("],", "],\n"));
        //System.out.print(s.hiddenSingles());
    }
}

