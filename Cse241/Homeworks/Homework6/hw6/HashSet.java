@SuppressWarnings("unchecked")

public class HashSet<E> implements Set<E>
{
    
    private E[] arr;
    private Iterator<E> it;
    
    
    /**
    * Initializes the HashSet and it's iterator for this object 
    */
    public HashSet()
    { 
        arr = (E[])new Object[0];
        it  = new Iterator<E>(arr);
    }
    
    /**
    * Initializes the HashSet and it's iterator with given parameter for this object
    * @param e This will be assigned as set's first element 
    */
    public HashSet(E e)
    { 
        arr = (E[])new Object[1];
        it  = new Iterator<E>(arr);
        arr[0] = e;
    }

    /**
     * Returns an Iterator of this HashSet object
     */
    public Iterator iterator()
    {
        it = new Iterator<E>(arr);

        return it;
    } 

    /**
     * After this method call, returned iterator points to first element of this object
     */
    public Iterator begin()
    {
        it = it.begin();

        return it;
    } 

    /**
     * Returns size of this HashSet object
     */
    public int size()
    {
        return arr.length;
    }

    /**
     * Inserts given element into this HashSet object
     * @param e 
     */
    public void add(E e) 
    {
        try 
        {
            // checking for duplicates
            checkDuplicates(e);

            // creating temp array to increase size by 1
            E[] tmp_arr = (E[]) new Object[arr.length+1];

            // inserting elements into temp
            for(int i=0; i<arr.length; i++)
                tmp_arr[i] = arr[i];

            // getting index of the last element
            int LastElementsIndex = arr.length;

            // inserting given parameter to the array
            tmp_arr[LastElementsIndex] = e;

            // assigning temp array into original one
            arr = tmp_arr;

        }
        catch(DuplicatedElement D)
        {   
            System.err.println("Exception occured: Given element \""+ e +"\" is already in the array.");
        }
    }
    
    /**
     * Adds all of the elements in the given collection into this HashSet object
     * @param c 
     */
    public void addAll(Collection c)
    {
        // creating an temporary iterator to hold c collection iterator
        Iterator tempIt = c.iterator();
        E tempData = (E)tempIt.begin();

        // adding every element that temp iterator holds into arr 
        while(tempIt.hasNext())
        {
            tempData = (E)tempIt.next();
            add(tempData);
        }    
    }

    /**
     * Removes all the elements from this HashSet object
     */
    public void clear()
    {
        // assigning iterator to null
        it = null;        
        
        // creating temp null arr
        E[] tempArr = (E[]) new Object[0];
        // pointing arr into that null arr
        arr = tempArr;
    }

    /**
     * Returns true, if the given element is in this HashSet object. Else returns false.
     * @param e 
     */
    public boolean contains(E e)
    {
        for(E temp : arr)
            if(temp == e)
                return true;
        
        return false;
    }

    /**
     * Returns true, if the given collection's elements are all exist in this HashSet object. Else returns false.
     * @param c 
     */
    public boolean containsAll(Collection c)
    {
        int count = 0;
        int collectionCount = 0;

        Iterator cIt = c.iterator();
        E cData;

        while(cIt.hasNext())
        {
            // getting element
            cData = (E)cIt.next();

            // checking if there is same element in arr
            for(int i=0; i<arr.length; i++)
                if(arr[i] == cData)
                    count++;

            collectionCount++;
        }

        if(count == collectionCount)
            return true;

        return false;
    }
   
    /**
     * Returns true, if the HashSet object doesn't contain any element.
     */
    public boolean isEmpty()
    {
        if(arr.length == 0)
            return true;

        return false;
    }

    /**
     * If it exists, removes given element from this HashSet object
     * @param e 
     */
    public void remove(E e)
    {
        try
        {
            if(contains(e) == false)
                throw new ElementNotExists();

            // temporary arrIt
            E[] temp_arr = arr;
            
            // decreasing size
            int size = temp_arr.length - 1;
    
            // allocating arr with decreased value
            arr = (E[]) new Object[size];
            
            int tempIndex = 0;
            // inserting all elements in temp_arrIt into arr, except element that we will remove
            for(int i=0; i<size+1; i++)
            {
                if(temp_arr[i] != e)
                {
                    arr[tempIndex] = temp_arr[i];  
                    tempIndex++;
                }
            }
        }
        catch(ElementNotExists E)
        {
            System.err.println("Exception occured: The element you're trying to remove doesn't exist in the array");
        }

    }

    /**
     * Removes every element from this object which also exist in the given Collection.
     * @param c 
     */
    public void removeAll(Collection c)
    {
        Iterator tempIterator = c.iterator();
        E tempData = (E)tempIterator.begin();
        
        while(tempIterator.hasNext())
        {   
            tempData = (E)tempIterator.next();
            remove(tempData);
        }
    }

    /**
     * Removes every element from this object which doesn't exist in the given Collection.
     * @param c 
     */
    public void retainAll(Collection c)
    {
        removeAll(this);
        addAll(c);
    }
   
    // Helper Methods

    /**
     * Throws an exception if the given element is already exist in this object.
     * @param e
     * @throws DuplicatedElement
     */
    private void checkDuplicates(E e) throws DuplicatedElement
    {
        for(E temp : arr)
        if(temp == e)
            throw new DuplicatedElement();
    }

    // ToString Function

    /**
     * Turns this object into string.
     * Prints every element with one space between them. ex: "1 2 3 4"
     */
    public String toString()
    {
        String temp = "";

        for(int i=0; i<arr.length; i++)
            temp += arr[i] + " ";

        return String.format(temp);
    }
}
