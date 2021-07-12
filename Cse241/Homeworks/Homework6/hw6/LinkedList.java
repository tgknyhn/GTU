import java.rmi.server.ObjID;

@SuppressWarnings("unchecked")

public class LinkedList<E> implements List<E>, Queue<E>
{
    private E tmp;
    private E[] arr;
    private Iterator<E> it;
    
    /**
    * Initializes the LinkedList and it's iterator for this object 
    */
    public LinkedList()
    { 
        arr = (E[])new Object[0];
        tmp = (E)new Object();
        it  = new Iterator<E>(arr);
    }
    
    /**
    * Initializes the ArrayList and it's iterator with given parameter for this object
    * @param e This will be assigned as set's first element 
    */
    public LinkedList(E e)
    { 
        arr = (E[])new Object[1];
        tmp = (E)new Object();
        it  = new Iterator<E>(arr);
        arr[0] = e;
    }

    /**
     * Returns an Iterator of this LinkedList object
     */
    public Iterator iterator()
    {
        it = new Iterator<E>(arr, this);

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
     * Returns size of this LinkedList object
     */
    public int size()
    {
        return arr.length;
    }

    /**
     * Throws exception. Please use offer(E e) method instead of this.
     * @param e 
     */    
    public void add(E e) 
    {
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

    /**
     * Inserts given element into given index position.
     */
    public void add(int index, E e) 
    {
        try
        {
            if(index < 0 || index >= arr.length)
                throw new OutOfBound();
                
            // creating temp array to increase size by 1
            E[] tmp_arr = (E[]) new Object[arr.length+1];

            // inserting elements into temp
            for(int i=0, j=0; i<tmp_arr.length; i++, j++)
            {
                if(i != index)
                    tmp_arr[i] = arr[j];
                else
                    j--;
            }

            // inserting E e into given index in tmp_arr
            tmp_arr[index] = e;

            // assigning temp array into original one
            arr = tmp_arr;
        }
        catch(OutOfBound O)
        {
            System.err.println("Exception occured: Given element could not inserted. Index is not valid");
        }        
    }
    
    /**
     * Adds all of the elements in the given collection into this ArrayList object
     * @param c 
     */   
    public void addAll(Collection c)
    {
        try
        {
            if(this instanceof Queue)
                throw new InvalidAccess();
        }
        catch(InvalidAccess I)
        {
            System.err.println("Exception occured: addAll(Collection c) is not supported with queue type classes");
        }  
    }

    /**
     * Removes all the elements from this LinkedList object
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
     * Returns true, if the given element is in this LinkedList object. Else returns false.
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
     * Returns true, if the given collection's elements are all exist in this LinkedList object. Else returns false.
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
     * Returns true, if the LinkedList object doesn't contain any element.
     */
    public boolean isEmpty()
    {
        if(arr.length == 0)
            return true;

        return false;
    }

    /**
     * If it exists, removes given element from this LinkedList object
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
            
            // inserting all elements in temp_arrIt into arr, except element that we will remove
            for(int i=0, j=0; i<size+1; i++)
            {
                if(temp_arr[i] != e)
                {
                    arr[j] = temp_arr[i];  
                    j++;
                }
                else
                    break;
            }
        }
        catch(ElementNotExists E)
        {
            System.err.println("Exception occured: The element you're trying to remove doesn't exist in the array");
        }
    }

    /**
     * Removes every element from this object that also exists in the given Collection.
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
   
    // queue implementation

    /**
     * Returns the element at the head of this LinkedList object
     * @return
     * @throws ElementNotExists
     */
    public E element() throws ElementNotExists
    {
        if(arr.length == 0)
            throw new ElementNotExists();

        return arr[0];
    }

    /**
     * Inserts given element into this LinkedList object
     * @param e
     */
    public void offer(E e)
    {
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

    /**
     * Returns element at the head of this LinkedList object. Also removes it.
     * @return
     * @throws ElementNotExists
     */
    public E poll() throws ElementNotExists
    {
        if(arr.length == 0)
            throw new ElementNotExists();

        // creating temp array to decrease size by 1
        E[] tmp_arr = (E[]) new Object[arr.length-1];

        // inserting elements into temp
        for(int i=1, j=0; i<arr.length; i++, j++)
            tmp_arr[j] = arr[i];

        E temp = arr[0];

        // assigning temp array into original one
        arr = tmp_arr;

        return temp;
    }

    // Element Getters and Setters

    /**
     * Throws exception. Please use element() method instead of this one.
     */
    public E get(int index) 
    {
        try
        {
            if(index < 0 || index >= arr.length)
                throw new ElementNotExists();
                
            return arr[index];
        }
        catch(ElementNotExists E)
        {
            System.err.println("Exception occured: There is no element in the given index. Returning empty element");
            return tmp;
        }
    }

    /**
     * Throws exception. This method is not usable for Queue type classes.
     */
    public void set(int index, E e) 
    {
        try
        {
            if(this instanceof Queue)
                throw new InvalidAccess();
        }
        catch(InvalidAccess I)
        {
            System.err.println("Exception occured: set(int index, E e) is not supported with queue type classes");
        }
    }

    /**
     * Returns the index of the given element.
     */
    public int indexOf(E e) 
    {
        try
        {
            if(this instanceof Queue)
                throw new InvalidAccess();
        }
        catch(InvalidAccess I)
        {
            System.err.println("Exception occured: indexOf(E e) is not supported with queue type classes");
        }
        return 0;
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
