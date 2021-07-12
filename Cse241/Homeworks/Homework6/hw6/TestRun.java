/**
 * Tests HashList.java, ArrayList.java, LinkedList.java methods. Also triggers exceptions to see how they are working. 
 */
public class TestRun
{
    public static void main(String[] args)
    {
        // [HashSet Testing]

        // 1) Test with HashSet<String>
        HashSetTest_1();

        // 2) Test with HashSet<Integer> & Iterator
        // since in HashSet<Integer> everything works same with HashSet<String> class 
        // this will only use Iterators that returned from a HashSet<Integer> class)
        HashSetTest_2();

        // 3) Triggering Exceptions
        HashSetTest_3();



        // [ArrayList Testing]

        // 1) Methods (different from HashSet) 
        // Except some functions every function works same with HashSet so we will only check to those
        ArrayListTest_1();

        // 2) Exceptions 
        // Showing only different exceptions from HashSet
        ArrayListTest_2();

        // [LinkedList Testing]

        // 1) Methods (different from both hashset and arraylist)
        LinkedListTest_1();

        // 2) Exceptions (different from both HashSet and ArrayList)
        LinkedListTest_2();
    }

    private static void HashSetTest_1()
    {
        // creating HashSet<String> class objects
        HashSet<String> hash1_str = new HashSet<String>("abc");
        HashSet<String> hash2_str = new HashSet<String>("def");
        HashSet<String> hash3_str = new HashSet<String>("xyz");

        // First, let's print all hashed without changing them 
        System.out.println("\n\n[ **  HashSet<String> Methods Showcase ** ]\n\n");
        System.out.println("-> Set1: " + hash1_str); // with toString function I can do that
        System.out.println("-> Set2: " + hash2_str); 
        System.out.println("-> Set3: " + hash3_str);
        System.out.println("");
        
        // [AddAll]

        System.out.println("[  AddAll Method  ]\n");
        // now, let's add hash2_str with hash1_str and print hash1_str again
        hash1_str.addAll(hash2_str);
        System.out.println("-> Set1: " + hash1_str); 

        // finally we add hash3_str with hash1_str and print hash1_str 
        hash1_str.addAll(hash3_str);
        System.out.println("-> Set1: " + hash1_str); 
        System.out.println("");
        
        // [Add]

        System.out.println("[  Add Method  ]\n");
        // Adding a const string into -> Set1
        hash1_str.add("123");
        System.out.println("-> Set1: " + hash1_str + "\n"); 

        // [Contains]

        System.out.println("[  Contains Method  ] (passed if condition)\n");
        // printing if the string contains "abc"
        if(hash1_str.contains("abc"))
            System.out.println("-> Set1: " + hash1_str + "\n"); 

        // [ContainsAll]

        System.out.println("[  ContainsAll Method  ] (passed if condition)\n");
        // let's create a string with exact content with -> Set1
        HashSet<String> DoContain = new HashSet<String>("");
        DoContain.add("def");
        DoContain.add("123");
        // now we try to print -> Set1 if the -> Set1 contains DoContain
        if(hash1_str.containsAll(DoContain))
            System.out.println("-> Set1: " + hash1_str + "\n"); 

        // [Remove]

        System.out.println("[  Remove Method  ] (\"This is a\" element removed)\n");
        // removing "abc"
        hash1_str.remove("abc");
        System.out.println("-> Set1: " + hash1_str + "\n"); 

        // [RemoveAll]

        System.out.println("[  RemoveAll Method  ] (Every element in \"removeThis\" class is removed)\n");
        // creating a hashSet with hash3_str + "This is added"
        HashSet<String> removeThis = new HashSet<String>();
        removeThis.add("123");
        removeThis.addAll(hash3_str);
        // removing that hashSet
        hash1_str.removeAll(removeThis);
        System.out.println("-> Set1: " + hash1_str + "\n");  

        // [RetainAll]

        System.out.println("[  RetainAll Method  ] (first string is all removed strings added back) (second one only retains -> Set3)\n");
        // adding every removed elements back and printing it
        hash1_str.addAll(removeThis);
        hash1_str.add("abc");
        System.out.println("-> Set1: " + hash1_str + "\n");  
        // 
        hash1_str.retainAll(hash3_str);
        System.out.println("-> Set1: " + hash1_str + "\n");  

        // [Size] [isEmpty]
        System.out.println("[  Size & isEmpty & Clean Methods  ] \n");
        // let's see what is the size of hash1_str without touching it
        System.out.print("-> Set1: " + hash1_str + "| size: " + hash1_str.size());
        // isEmpty check
        if(hash1_str.isEmpty() == true)
            System.out.println(" | isEmpty: Yes\n");
        else    
            System.out.println(" | isEmpty: No\n");
        // adding two to hash1_str
        hash1_str.add("nml");
        hash1_str.add("pqr");
        // now let's check size
        System.out.print("-> Set1: " + hash1_str + "| size: " + hash1_str.size());
        if(hash1_str.isEmpty() == true)
            System.out.println(" | isEmpty: Yes\n");
        else    
            System.out.println(" | isEmpty: No\n");
        // lets clean the hash1_str and then print
        hash1_str.clear();
        System.out.print("-> Set1: " + hash1_str + "| size: " + hash1_str.size());
        if(hash1_str.isEmpty() == true)
            System.out.print(" | isEmpty: Yes\n");
        else    
            System.out.print(" | isEmpty: No\n");
            
        System.out.println("\n\n[ ** END ** ]\n\n");

    }

    private static void HashSetTest_2()
    {
        System.out.println("\n\n[ **  HashSet<Integer> Iterator Showcase ** ]\n\n");

        // allocating hash1
        HashSet<Integer> hash_int = new HashSet<Integer>();
        // adding elements to it
        hash_int.add(10);
        hash_int.add(45);
        hash_int.add(238);
        // getting iterator of this collection
        Iterator it = hash_int.iterator();
        it = it.begin();

        // First, let's print what is inside
        System.out.print("-> Set: ");
        while(it.hasNext())
        {
            System.out.print(" " + it.next());
        }
        System.out.println();

        // now, time to remove elements
        it = it.begin();
        it.remove();
        System.out.print("-> Set: ");
        while(it.hasNext())
        {
            System.out.print(" " + it.next());
        }
        System.out.println();

        
        // remove again.
        it = it.begin();
        it.remove();
        System.out.print("-> Set: ");
        while(it.hasNext())
        {
            System.out.print(" " + it.next());
        }
        System.out.println();

        System.out.println("\n\n[ ** END ** ]\n\n");
    }

    private static void HashSetTest_3()
    {
        System.out.println("\n\n[ **  HashSet<E> Exceptions Showcase ** ]\n\n");

        HashSet<Integer> setInt = new HashSet<Integer>();
        // Starting with duplicates
        setInt.add(10);  
        setInt.add(10);
        
        // Let's try to remove an element that doesn't exist on the set
        setInt.remove(251);
        
        // now iterator exception
        Iterator SetIt = setInt.iterator();
        SetIt = SetIt.begin();

        // trying to remove whilst there is no element on the set
        SetIt.remove(); // removes 10
        SetIt.remove(); // throws exception
    
        System.out.println("\n\n[ ** END ** ]\n\n");
    }

    private static void ArrayListTest_1()
    {
        System.out.println("\n\n[ **  ArrayList<String> Methods Showcase ** ]\n\n");

        ArrayList<String> arr = new ArrayList<String>();

        // add set get indexof

        // first, let's add 3 random elements into arr
        arr.add("123");
        arr.add("456");
        arr.add("123");
        arr.add("789");
        
        System.out.print("-> Set:");
        // different from HashSet, user can reach elements without toString function
        for(int i=0; i<arr.size(); i++)
            System.out.print(" " + arr.get(i));
        System.out.println("\t\t\t (In this Collection we can add same elements) (this ex: \"123\") \n");

        // [new Add function]
        arr.add(1, "abc");
        System.out.print("-> Set:");
        // different from HashSet, user can reach elements without toString function
        for(int i=0; i<arr.size(); i++)
            System.out.print(" " + arr.get(i));
        System.out.println("\t\t [ add(int index, E e) function ] (adds elemet into given index) (index = 1, e = \"abc\")\n");

        // [new set function]        
        arr.set(1, "789");

        // first swap
        System.out.print("-> Set:");
        // different from HashSet, user can reach elements without toString function
        for(int i=0; i<arr.size(); i++)
            System.out.print(" " + arr.get(i));
        System.out.println("\t\t [ set(int index, E e) function ] (swaps an existing elemet into given index)\n");
        
        // second swap
        arr.set(2, "789");
        System.out.print("-> Set:");
        // different from HashSet, user can reach elements without toString function
        for(int i=0; i<arr.size(); i++)
            System.out.print(" " + arr.get(i));
        System.out.println("\t\t [ set(int index, E e) function ]");

        System.out.println("\n\n[ ** END ** ]\n\n");
    }

    private static void ArrayListTest_2()
    {
        System.out.println("\n\n[ **  ArrayList<Integer> Exceptions Showcase ** ]\n\n");

        // first, creating an ArrayList
        ArrayList<Integer> arrInt = new ArrayList<Integer>();
        
        // first let's add some elements to it
        arrInt.add(11);
        arrInt.add(0, 9);

        // add set get indexof

        // "add" function's exception throw
        arrInt.add(5, 10);

        // "set" function's exception throw
        arrInt.set(5, 10);

        // "get" function's exception throw
        arrInt.get(5);
        
        System.out.println("\n\n[ ** END ** ]\n\n");
    }

    private static void LinkedListTest_1()
    {
        System.out.println("\n\n[ **  LinkedList<String> Methods Showcase ** ]\n\n");


        // first, creating an LinkedList
        LinkedList<String> llStr = new LinkedList<String>();

        try
        {
            // [ offer(E e) ]
            System.out.println("[ offer Method ]\n");
            llStr.offer("xyz");
            System.out.println("-> Set: " + llStr + "\t\t (xyz added to linkedlist)");
            llStr.offer("klm");
            System.out.println("-> Set: " + llStr + "\t (klm added to linkedlist)");
            llStr.offer("abc");
            System.out.println("-> Set: " + llStr + "\t (abc added to linkedlist)");

            // [ elements() ]
            System.out.println("\n[ element Method ] (only returns head of queue which is xyz)\n");
            System.out.println("-> Set: " + llStr.element());
            
            // [ poll() ]
            System.out.println("\n[ poll Method ] \n");
            llStr.poll();
            System.out.println("-> Set: " + llStr + "\t (xyz removed)");
            llStr.poll();
            System.out.println("-> Set: " + llStr + "\t\t (klm removed)");
            llStr.poll();
            System.out.println("-> Set: " + llStr + "\t\t (abc removed)");
            
            System.out.println("\n\n[ ** END ** ]\n\n");
        }
        catch(ElementNotExists E)
        {
            System.err.println("Exception occured: There no element to show in the Linked list.");
        }        
    }
        
    private static void LinkedListTest_2()
    {
        System.out.println("\n\n[ **  LinkedList<Integer> Exceptions Showcase ** ]\n\n");

        // first, creating an LinkedList
        LinkedList<Integer> llStr = new LinkedList<Integer>();

        try
        {
            llStr.element();            
        }
        catch(ElementNotExists E)
        {
            System.err.println("Exception occured: (Element)\tThere is no element left in the list. ");
        }
        try
        {
            llStr.poll();            
        }
        catch(ElementNotExists E)
        {
            System.err.println("Exception occured: (Poll)\tThere is no element left in the list. ");
        }
        
        Iterator llIt = llStr.iterator();
        // iterator exception for remove
        llIt.remove();

        System.out.println("\n\n[ ** END ** ]\n\n");

    }
}