package alg;

import java.io.IOException;

public class Main {
    
    public static void main(String[] args) throws IOException {
        Agents.loadData(null);
        int[] result = Agents.solve();
        System.out.println(result[0]);
    }
}
