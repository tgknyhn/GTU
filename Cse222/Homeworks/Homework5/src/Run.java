import java.util.Random;

/**
 * Driver class for Homework 5
 */
public class Run {
    public static void main(String[] args) {

        // ***********************
        // *  PART 1 Test Cases  *
        // ***********************

        System.out.println();
        System.out.println("************");
        System.out.println("** PART 1 **");
        System.out.println("************");

        // Initializing myHash object
        HashtableOpen<String, String> myHashOpen = new HashtableOpen<>();

        System.out.println("\nAdding elements...\n");
        // Inserting some keys and values
        myHashOpen.put("türkiye", "ankara");
        myHashOpen.put("almanya", "berlin");
        myHashOpen.put("fransa", "paris");
        myHashOpen.put("abd", "washington");
        myHashOpen.put("ingiltere", "londra");
        myHashOpen.put("japonya", "tokyo");

        // Getting the Iterator of the myHashOpen object with no parameter constructor
        HashtableOpen<String, String >.MapIterator myItr = myHashOpen.iterator();

        // printing all elements
        System.out.println("With no parameter constructor");
        System.out.println("-----------------------------");

        System.out.print("Elements (next method) : " + myItr.next());
        while (myItr.hasNext())
            System.out.print(", " + myItr.next());

        // Getting elements with prev method
        System.out.print("\nElements (prev method) : " + myItr.prev());
        for(int i=0; i< myHashOpen.size()-1; i++)
            System.out.print(", " + myItr.prev());

        //

        // Getting the Iterator of the myHashOpen object with one parameter constructor
        myItr = myHashOpen.iterator("japonya");

        // printing all elements
        System.out.println("\n\nWith one parameter constructor(I choosed \"japonya\")");
        System.out.println("----------------------------------------------------");


        System.out.print("Elements (next method) : " + myItr.next());
        while (myItr.hasNext())
            System.out.print(", " + myItr.next());

        // Getting elements with prev method
        System.out.print("\nElements (prev method) : " + myItr.prev());
        for(int i=0; i< myHashOpen.size()-1; i++)
            System.out.print(", " + myItr.prev());


        // ***********************
        // *  PART 2 Test Cases  *
        // ***********************

        System.out.println("\n\n");
        System.out.println("************");
        System.out.println("** PART 2 **");
        System.out.println("************");

        // initializing KWHashMap objects
        KWHashMap<Integer, String> myChain     = new HashtableChain<>();
        KWHashMap<Integer, String> myTree      = new HashtableTree<>();
        KWHashMap<Integer, String> myCoalesced = new HashtableCoalesced<>();

        // time holders
        long timeChain;
        long timeTree;
        long timeCoalesced;

        // bound for rand numbers
        final int bound = 1000;

        // random numbers
        int[] small  = new int[5];
        int[] medium = new int[100];
        int[] large  = new int[2000];

        // random seed
        Random random = new Random();

        // filling random number arrays
        for(int i=0; i<small.length; i++)
            small[i] = random.nextInt(bound);
        for(int i=0; i<medium.length; i++)
            medium[i] = random.nextInt(bound);
        for(int i=0; i< large.length; i++)
            large[i] = random.nextInt(bound);

        // getting existing / non existing elements
        int[] existingSmall  = new int[2];
        int[] existingMedium = new int[10];
        int[] existingLarge  = new int[100];
        int   nonExisting    = bound + 99;

        // filling existing elements
        for(int i=0; i<existingSmall.length; i++)
            existingSmall[i] = small[i];
        for(int i=0; i<existingMedium.length; i++)
            existingMedium[i] = medium[i];
        for(int i=0; i<existingLarge.length; i++)
            existingLarge[i] = large[i];


        System.out.println("\nInserting 5 elements into hashTables");
        System.out.println("------------------------------------");

        System.out.println("\nInserting...");

        timeChain = System.nanoTime();
        for (int j : small) myChain.put     (j, Integer.toString(j-10));
        timeChain = System.nanoTime() - timeChain;

        timeTree = System.nanoTime();
        for (int j : small) myTree.put      (j, Integer.toString(j-10));
        timeTree = System.nanoTime() - timeTree;

        timeCoalesced = System.nanoTime();
        for (int j : small) myCoalesced.put (j, Integer.toString(j-10));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        // printing out hashmaps
        System.out.println("\n-- HashtableChain --");
        System.out.println(myChain);

        System.out.println("\n-- HashtableTree --");
        System.out.println(myTree);

        System.out.println("\n-- HashtableCoalesced -- \n");
        System.out.println(myCoalesced);

        System.out.println("\nTime collapsed while inserting 5 elements into HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while inserting 5 elements into HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while inserting 5 elements into HashtableCoalesced object : " + timeCoalesced + " nanosecond");

        System.out.println("\nGetting 2 elements which exist in hashTables");
        System.out.println("---------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        for (int k : existingSmall) System.out.println("HashtableChain     -> Key: " + k + " Value: " + myChain.get(k));
        timeChain = System.nanoTime() - timeChain;

        System.out.println();

        timeTree = System.nanoTime();
        for (int k : existingSmall) System.out.println("HashtableTree      -> Key: " + k + " Value: " + myTree.get(k));
        timeTree = System.nanoTime() - timeTree;

        System.out.println();

        timeCoalesced = System.nanoTime();
        for (int k : existingSmall) System.out.println("HashtableCoalesced -> Key: " + k + " Value: " + myCoalesced.get(k));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting 2 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting 2 elements that exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while getting 2 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nGetting one element which not exist in hashTables");
        System.out.println("------------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        System.out.println("HashtableChain     -> Key: " + nonExisting + " Value: " + myChain.get(nonExisting));
        timeChain = System.nanoTime() - timeChain;

        System.out.println();

        timeTree = System.nanoTime();
        System.out.println("HashtableTree      -> Key: " + nonExisting + " Value: " + myTree.get(nonExisting));
        timeTree = System.nanoTime() - timeTree;

        System.out.println();

        timeCoalesced = System.nanoTime();
        System.out.println("HashtableCoalesced -> Key: " + nonExisting + " Value: " + myCoalesced.get(nonExisting));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting one element which not exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving 2 element that exist in hashTables");
        System.out.println("-------------------------------------------");

        System.out.println("\nRemoving... (" + existingSmall[0] + ", " + existingSmall[1] + ")" );

        int sizeBeforeRem_chain,     sizeAfterRem_chain;
        int sizeBeforeRem_tree,      sizeAfterRem_tree;
        int sizeBeforeRem_coalesced, sizeAfterRem_coalesced;

        System.out.println();

        sizeBeforeRem_chain = myChain.size();
        timeChain = System.nanoTime();
        for(int i=0; i<existingSmall.length; i++)
            myChain.remove(existingSmall[i]);
        timeChain = System.nanoTime() - timeChain;
        sizeAfterRem_chain = myChain.size();

        sizeBeforeRem_tree = myTree.size();
        timeTree = System.nanoTime();
        for(int i=0; i<existingSmall.length; i++)
            myTree.remove(existingSmall[i]);
        timeTree = System.nanoTime() - timeTree;
        sizeAfterRem_tree = myTree.size();

        sizeBeforeRem_coalesced = myCoalesced.size();
        timeCoalesced = System.nanoTime();
        for(int i=0; i<existingSmall.length; i++)
           myCoalesced.remove(existingSmall[i]);
        timeCoalesced = System.nanoTime() - timeCoalesced;
        sizeAfterRem_coalesced = myCoalesced.size();

        System.out.println("(HashtableChain)     Size before removing : " + sizeBeforeRem_chain     + " Size after removing : " + sizeAfterRem_chain);
        System.out.println("(HashtableTree)      Size before removing : " + sizeBeforeRem_tree      + " Size after removing : " + sizeAfterRem_tree);
        System.out.println("(HashtableCoalesced) Size before removing : " + sizeBeforeRem_coalesced + " Size after removing : " + sizeAfterRem_coalesced);

        System.out.println("\nTime collapsed while removing 2 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while removing 2 elements that exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while removing 2 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        // MEDIUM DATA

        // initializing KWHashMap objects
        myChain     = new HashtableChain<>();
        myTree      = new HashtableTree<>();
        myCoalesced = new HashtableCoalesced<>();


        System.out.println("\nInserting 100 elements into hashTables");
        System.out.println("------------------------------------");

        System.out.println("\nInserting...");

        timeChain = System.nanoTime();
        for (int j : medium) myChain.put     (j, Integer.toString(j-10));
        timeChain = System.nanoTime() - timeChain;

        timeTree = System.nanoTime();
        for (int j : medium) myTree.put      (j, Integer.toString(j-10));
        timeTree = System.nanoTime() - timeTree;

        timeCoalesced = System.nanoTime();
        for (int j : medium) myCoalesced.put (j, Integer.toString(j-10));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while inserting 100 elements into HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while inserting 100 elements into HashtableTree object       : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while inserting 100 elements into HashtableCoalesced object : " + timeCoalesced + " nanosecond");

        System.out.println("\nGetting 10 elements which exist in hashTables");
        System.out.println("----------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        for (int k : existingMedium) myChain.get(k);
        timeChain = System.nanoTime() - timeChain;

        timeTree = System.nanoTime();
        for (int k : existingMedium) myTree.get(k);
        timeTree = System.nanoTime() - timeTree;

        timeCoalesced = System.nanoTime();
        for (int k : existingMedium) myCoalesced.get(k);
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting 2 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting 2 elements that exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while getting 2 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nGetting one element which not exist in hashTables");
        System.out.println("------------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        System.out.println("HashtableChain     -> Key: " + nonExisting + " Value: " + myChain.get(nonExisting));
        timeChain = System.nanoTime() - timeChain;

        System.out.println();

        timeTree = System.nanoTime();
        System.out.println("HashtableTree      -> Key: " + nonExisting + " Value: " + myTree.get(nonExisting));
        timeTree = System.nanoTime() - timeTree;

        System.out.println();

        timeCoalesced = System.nanoTime();
        System.out.println("HashtableCoalesced -> Key: " + nonExisting + " Value: " + myCoalesced.get(nonExisting));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting one element which not exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving 10 element that exist in hashTables");
        System.out.println("--------------------------------------------");

        System.out.println("\nRemoving..." );

        System.out.println();

        sizeBeforeRem_chain = myChain.size();
        timeChain = System.nanoTime();
        for(int i=0; i<existingMedium.length; i++)
            myChain.remove(existingMedium[i]);
        timeChain = System.nanoTime() - timeChain;
        sizeAfterRem_chain = myChain.size();

        sizeBeforeRem_tree = myTree.size();
        timeTree = System.nanoTime();
        for(int i=0; i<existingMedium.length; i++)
            myTree.remove(existingMedium[i]);
        timeTree = System.nanoTime() - timeTree;
        sizeAfterRem_tree = myTree.size();

        sizeBeforeRem_coalesced = myCoalesced.size();
        timeCoalesced = System.nanoTime();
        for(int i=0; i<existingMedium.length; i++)
            myCoalesced.remove(existingMedium[i]);
        timeCoalesced = System.nanoTime() - timeCoalesced;
        sizeAfterRem_coalesced = myCoalesced.size();

        System.out.println("(HashtableChain)     Size before removing : " + sizeBeforeRem_chain     + " Size after removing : " + sizeAfterRem_chain);
        System.out.println("(HashtableTree)      Size before removing : " + sizeBeforeRem_tree      + " Size after removing : " + sizeAfterRem_tree);
        System.out.println("(HashtableCoalesced) Size before removing : " + sizeBeforeRem_coalesced + " Size after removing : " + sizeAfterRem_coalesced);

        System.out.println("\nTime collapsed while removing 10 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while removing 10 elements that exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while removing 10 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        // LARGE DATA

        // initializing KWHashMap objects
        myChain     = new HashtableChain<>();
        myTree      = new HashtableTree<>();
        myCoalesced = new HashtableCoalesced<>();


        System.out.println("\nInserting 2000 elements into hashTables");
        System.out.println("------------------------------------");

        System.out.println("\nInserting...");

        timeChain = System.nanoTime();
        for (int j : large) myChain.put     (j, Integer.toString(j-10));
        timeChain = System.nanoTime() - timeChain;

        timeTree = System.nanoTime();
        for (int j : large) myTree.put      (j, Integer.toString(j-10));
        timeTree = System.nanoTime() - timeTree;

        timeCoalesced = System.nanoTime();
        for (int j : large) myCoalesced.put (j, Integer.toString(j-10));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while inserting 2000 elements into HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while inserting 2000 elements into HashtableTree object       : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while inserting 2000 elements into HashtableCoalesced object : " + timeCoalesced + " nanosecond");

        System.out.println("\nGetting 100 elements which exist in hashTables");
        System.out.println("------------*----------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        for (int k : existingLarge) myChain.get(k);
        timeChain = System.nanoTime() - timeChain;

        timeTree = System.nanoTime();
        for (int k : existingLarge) myTree.get(k);
        timeTree = System.nanoTime() - timeTree;

        timeCoalesced = System.nanoTime();
        for (int k : existingLarge) myCoalesced.get(k);
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting 100 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting 100 elements that exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while getting 100 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nGetting one element which not exist in hashTables");
        System.out.println("------------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        System.out.println("HashtableChain     -> Key: " + nonExisting + " Value: " + myChain.get(nonExisting));
        timeChain = System.nanoTime() - timeChain;

        System.out.println();

        timeTree = System.nanoTime();
        System.out.println("HashtableTree      -> Key: " + nonExisting + " Value: " + myTree.get(nonExisting));
        timeTree = System.nanoTime() - timeTree;

        System.out.println();

        timeCoalesced = System.nanoTime();
        System.out.println("HashtableCoalesced -> Key: " + nonExisting + " Value: " + myCoalesced.get(nonExisting));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting one element which not exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving 500 element that exist in hashTables");
        System.out.println("---------------------------------------------");

        System.out.println("\nRemoving..." );

        System.out.println();

        timeChain = System.nanoTime();
        for(int i=0; i<existingLarge.length; i++) {
            try {
                myChain.remove(existingLarge[i]);
            } catch (NullPointerException ignore) { }
        }
        timeChain = System.nanoTime() - timeChain;

        timeTree = System.nanoTime();
        for(int i=0; i<existingLarge.length; i++)
            try {
                myTree.remove(existingLarge[i]);
            } catch (NullPointerException ignore) { }
        timeTree = System.nanoTime() - timeTree;

        timeCoalesced = System.nanoTime();
        for(int i=0; i<existingLarge.length; i++)
            try {
                myCoalesced.remove(existingLarge[i]);
            } catch (NullPointerException ignore) { }
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("Time collapsed while removing 100 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while removing 100 elements that exist in HashtableTree object      : " + timeTree + " nanosecond");
        System.out.println("Time collapsed while removing 100 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving one element that doesn't exist in hashTables");
        System.out.println("----------------------------------------------------");
        try {
            myChain.remove(nonExisting);
        } catch (NullPointerException n) {
            System.err.println("The item you are trying to remove doesn't exist in the hashmap");
        }
        try {
            myTree.remove(nonExisting);
        } catch (NullPointerException n) {
            System.err.println("The item you are trying to remove doesn't exist in the hashmap");
        }
        try {
            myCoalesced.remove(nonExisting);
        } catch (NullPointerException n) {
            System.err.println("The item you are trying to remove doesn't exist in the hashmap");
        }
    }
}
