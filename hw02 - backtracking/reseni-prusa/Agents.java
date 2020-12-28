package alg;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Agents {
    
    public static final int WHITE = 0;
    public static final int RED = 1;
    public static final int BLUE = 2;
    
    public static Node[] nodes;
    public static int numOfReds, numOfBlues;
    public static int[] degSums;
    public static int bestScore;
    public static int leavesCount;
    
    public static int maxN = 0, maxM = 0;
    
    public static class Node implements Comparable {
        int id;
        int color;
        int deg;
        List<Node> neighborsList;
        Node[] neighbors;
        
        public Node(int id) {
            this.id = id;
            neighborsList = new ArrayList<>();
        }
        
        public void init() {
            deg = neighborsList.size();
            neighbors = new Node[deg];
            int index = 0;
            for (Node node : neighborsList)
                neighbors[index++] = node;
        }

        @Override
        public int compareTo(Object o) {
            Node n = (Node)o;
            return n.deg - this.deg;
        }

    }
    
    public static void printColors() {
        String[] names = {"w", "r", "b"};
        for (Node node : nodes)
            System.out.print(node.id + ":" + names[node.color] + " ");
        System.out.println();
    }
    
    public static int score() {
        int sc = 0;
        for (Node node : nodes) {
            int rn = 0, bn = 0;
            for (Node n : node.neighbors)
                if (n.color == RED)
                    rn++;
                else if (n.color == BLUE)
                    bn++;
            sc += node.color == WHITE ? bn : rn;
        }
        return sc;
    }
    
    public static void initDegSums() {
        int nodesNum = nodes.length;
        degSums = new int[nodesNum+1];
        for (int i = nodesNum - 1; i >= 0; i--)
            degSums[i] = degSums[i+1] + nodes[i].deg;
    }
    
    public static void simpleRec(int index, int reds, int blues) {
        if (reds + blues == 0) {
            int score = score();
            if (score > bestScore) {
                bestScore = score;
                //System.out.println("score=" + bestScore);
                //printColors();
            }
            leavesCount++;
            return;
        }
        if (reds > 0) {
            nodes[index].color = RED;
            simpleRec(index+1, reds-1, blues);
            nodes[index].color = WHITE;
        }
        if (blues > 0) {
            nodes[index].color = BLUE;
            simpleRec(index+1, reds, blues-1);
            nodes[index].color = WHITE;
        }
        if (nodes.length - index - 1 >= reds + blues) {
            simpleRec(index+1, reds, blues);
        }
    }
    
    public static void advRec(int index, int reds, int blues, int score) {
        if (reds + blues == 0) {
            if (score > bestScore) {
                bestScore = score;
//                System.out.println("score=" + bestScore);
//                printColors();
            }
            leavesCount++;
            return;
        }
        // check if we can prune
        if (bestScore > score + 2*(degSums[index] - degSums[index + reds]) + (degSums[index + reds] - degSums[index + reds + blues])) {
            leavesCount++;
            return;
        }
        if (reds > 0) {
            nodes[index].color = RED;
            int delta = 0;
            for (Node node : nodes[index].neighbors)
                if (node.color == RED)
                    delta += 2;
            advRec(index+1, reds-1, blues, score + delta);
            nodes[index].color = WHITE;
        }
        if (blues > 0) {
            nodes[index].color = BLUE;
            int delta = 0;
            for (Node node : nodes[index].neighbors)
                if (node.color == BLUE)
                    delta--;
                else
                    delta++;
            advRec(index+1, reds, blues-1, score + delta);
            nodes[index].color = WHITE;
        }
        if (nodes.length - index - 1 >= reds + blues) {
            advRec(index+1, reds, blues, score);
        }
    }
    
    public static int[] solve() {
        for (Node node : nodes)
            node.init();
//        bestScore = 0;
//        leavesCount = 0;
//        System.out.println("simple recursion");
//        simpleRec(0, numOfReds, numOfBlues);
//        System.out.println("BEST SCORE = " + bestScore + ", LEAVES COUNT = " + leavesCount);
        
        Arrays.sort(nodes);
        initDegSums();
        bestScore = 0;
        leavesCount = 0;
        //System.out.println("advanced recursion");
        advRec(0, numOfReds, numOfBlues, 0);
        //System.out.println("BEST SCORE = " + bestScore + ", LEAVES COUNT = " + leavesCount);
        
        return new int[] {bestScore};
    }
    
    public static void loadData(String name) throws IOException {
        BufferedReader reader = name == null ? new BufferedReader(new InputStreamReader(System.in)) : new BufferedReader(new FileReader(name));
        StringTokenizer tokenizer = new StringTokenizer(reader.readLine());
        int N = Integer.valueOf(tokenizer.nextToken()); // #nodes
        int M = Integer.valueOf(tokenizer.nextToken()); // #edges
        numOfReds = Integer.valueOf(tokenizer.nextToken()); // #reds
        numOfBlues = Integer.valueOf(tokenizer.nextToken()); // #blues

        if (N > maxN)
            maxN = N;
        if (M > maxM)
            maxM = M;
        
        nodes = new Node[N];
        for (int i = 0; i < N; i++)
            nodes[i] = new Node(i+1);

        for (int i = 0; i < M; i++) {
            tokenizer = new StringTokenizer(reader.readLine());
            int from = Integer.valueOf(tokenizer.nextToken()) - 1;
            int to = Integer.valueOf(tokenizer.nextToken()) - 1;
            nodes[from].neighborsList.add(nodes[to]);
            nodes[to].neighborsList.add(nodes[from]);
        }
    }
    
    // data generator ***********************************************************
    
    public static void generateData(String fileName, long seed, int N, int M, int R, int B, boolean isDense) {
        Random rand = new Random(seed);
        Set<int[]> edges = new HashSet<>();
        nodes = new Node[N];
        for (int i = 0; i < N; i++)
            nodes[i] = new Node(i);
        for (int i = 1; i < N; i++) {
            int index = rand.nextInt(i);
            nodes[i].neighborsList.add(nodes[index]);
            nodes[index].neighborsList.add(nodes[i]);
            if (rand.nextBoolean())
                edges.add(new int[] {i, index});
            else
                edges.add(new int[] {index, i});
        }
        int counter = 0;
        while (edges.size() < M && counter < 20*M) {
            counter++;
            int n1 = rand.nextInt(N);
            int n2 = rand.nextInt(N);
            if (n1 == n2 || nodes[n1].neighborsList.contains(nodes[n2]))
                continue;
            nodes[n1].neighborsList.add(nodes[n2]);
            nodes[n2].neighborsList.add(nodes[n1]);
            edges.add(new int[] {n1, n2});
        }
        if (isDense) {
            int num = (R+B)/2+rand.nextInt(1+N-(R+B)/2);
            if (num > N)
                num = N;
            int degInc = N-1-nodes[0].neighborsList.size();
            for (int i = 0; i < num; i++) {
                for (int j = 0; j < degInc; j++) {
                    int ni = rand.nextInt(N);
                    if (i == ni || nodes[i].neighborsList.contains(nodes[ni]))
                        continue;
                    nodes[i].neighborsList.add(nodes[ni]);
                    nodes[ni].neighborsList.add(nodes[i]);
                    edges.add(new int[] {i, ni});
                }
            }
        }
        try {
            PrintWriter fout = new PrintWriter(fileName + ".in");
            fout.println(N + " " + edges.size() + " " + R + " " + B);
            for (int[] e : edges)
                fout.println((e[0]+1) + " " + (e[1]+1));
            fout.close();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(Agents.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public static void generateData() {
        // String fileName, long seed, int N, int M, int R, int B
        generateData("pub03", 3227128, 10, 17, 3, 4, false);
        generateData("pub04", 4402726, 12, 14, 3, 2, true);
        generateData("pub05", 7813892, 14, 91, 7, 5, false);
        generateData("pub06", 3302801, 20, 161, 10, 10, false);
        generateData("pub07", 3701996, 20, 173, 2, 5, true);
        generateData("pub08", 5533201, 26, 37, 8, 9, false);
        generateData("pub09", 1203401, 27, 78, 3, 8, false);
        generateData("pub10", 3301878, 22, 45, 7, 4, true);

        generateData("test01", 1124393, 8, 27, 4, 4, false);
        generateData("test02", 3300001, 12, 12, 5, 3, false);
        generateData("test03", 8934321, 14, 71, 6, 5, false);
        generateData("test04", 1302536, 22, 80, 14, 1, true);
        generateData("test05", 5307935, 18, 35, 5, 5, true);
        generateData("test06", 5538751, 19, 145, 6, 11, false);
        generateData("test07", 3231996, 22, 133, 4, 3, true);
        generateData("test08", 1339221, 25, 108, 2, 10, false);
        generateData("test09", 7739201, 30, 78, 5, 5, false);
        generateData("test10", 3301117, 25, 35, 11, 4, true);
    }
    
    public static void generateOutput(String fileName, int[] result) {
        try {
            PrintWriter fout = new PrintWriter(fileName + ".out");
            fout.println(result[0]);
            fout.close();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(Agents.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public static void main(String[] args) throws IOException {
        generateData();
        String[] prefix = {"pub", "test"};
        for (int iter = 1; iter <= 10; iter++) {
            for (int i = 0; i < 1; i++) {
                String name = prefix[i] + (iter < 10 ? "0" + iter : "" + iter);
                long time = System.currentTimeMillis();
                loadData(name + ".in");
                //long time = System.currentTimeMillis();
                int[] result = solve();
                time = System.currentTimeMillis() - time;
                System.out.println(name + " " + time + " [ms]");
                System.out.println(result[0]);
                
                //System.out.println("[" + name + "]");
                //System.out.println("description=\"M = " + nodes.length + ", T = " + E + ", N = " + roots.length + ", P = " + T + "\"");
                
                generateOutput(name, result);
            }
        }
        System.out.println("\n" + "N=" + maxN + ", M=" + maxM);
    }

}
