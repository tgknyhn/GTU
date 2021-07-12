@SuppressWarnings("unchecked")

/**
 * Keeps track of assigned Collection's elements
 */
public class Iterator<E> 
{
    private E data;
    private E[] arrIt;
    private int dataIndex;
    private boolean isQueue = false;

    /**
     * Initializes Iterator class
     */
    public Iterator()
    {
        dataIndex = 0;
        data      = null;
        arrIt     = null;
    }

    /**
     * Initializes Iterator class with given elements. So Iterator object knows which elements to track. 
     * @param newArrIt
     */
    public Iterator(E[] newArrIt)
    {
        dataIndex = (newArrIt.length > 0) ? newArrIt.length-1 : 0;
        data  = (newArrIt.length > 0) ? newArrIt[dataIndex] : null;
        arrIt = (newArrIt.length > 0) ? newArrIt : null;
    }

    /**
     * Initializes Iterator class with given elements. So Iterator object knows which elements to track. 
     * Also Iterator knows what kind of class is keeping elements. So it can prevent from invalid access.
     * @param newArrIt
     */
    public Iterator(E[] newArrIt, Collection c)
    {
        if(c instanceof Queue)
            isQueue = true;
        dataIndex = (newArrIt.length > 0) ? newArrIt.length-1 : 0;
        data  = (newArrIt.length > 0) ? newArrIt[dataIndex] : null;
        arrIt = (newArrIt.length > 0) ? newArrIt : null;
    }

    /**
     * Returns iterator's index into beggining. So it can point to first element again.
     * @return
     */
    public Iterator begin()
    {
        dataIndex = 0;
        data = arrIt[dataIndex];

        return this;
    }

    /**
     * Returns true, if iterator doesn't point at the end of the elements.
     * @return
     */
    public boolean hasNext()
    {
        if(dataIndex == arrIt.length)
            return false;
        
        return true;
    }   

    /**
     * Returns the element that iterator points and moves onto next element.
     * @return
     */
    public E next() 
    {
        try
        {
            if(hasNext() == false)
                throw new NoNextElement();
                
            
            data = arrIt[dataIndex];
            dataIndex++;
            
         
            return data;
        }
        catch(NoNextElement N)
        {
            System.err.println("Exception occured: Next element doesn't exist. Returning null.");
            return null;
        }
    }

    /**
     * Removes the element which iterator currently points into. 
     */
    public void remove()
    {
        try
        {
            if(isQueue == true)
                throw new InvalidAccess();

            if(arrIt.length == 0)
                throw new NoElementToRemove();

            // temporary arrIt
            E[] temp_arrIt = arrIt;
           
            // decreased size
            int size = temp_arrIt.length - 1;
    
            // allocating arrIt with decreased value
            arrIt = (E[]) new Object[size];
            
            int tempIndex = 0;
            // inserting all elements in temp_arrIt into arrIt, except element that we will remove
            for(int i=0; i<size+1; i++)
            {
                if(temp_arrIt[i] != temp_arrIt[dataIndex])
                {
                    arrIt[tempIndex] = temp_arrIt[i];  
                    tempIndex++;
                }
            }
        }
        catch(InvalidAccess I)
        {   
            System.err.println("Exception occured: iterator's Remove() function is not supported with queue type classes");
        }
        catch(NoElementToRemove N)
        {
            System.err.println("Exception occured: There is no element to remove.");
        }
    }
}
