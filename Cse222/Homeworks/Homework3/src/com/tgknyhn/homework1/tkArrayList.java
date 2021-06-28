package com.tgknyhn.homework1;

import java.util.Arrays;

/**
 * Arraylist for users in a Furniture Company.
 * @param <E> User type
 */
public class tkArrayList<E>{
    private static final int INITIAL_CAPACITY = 3;
    private E[] data;
    private int size = 0;
    private int capacity = 0;

    /**
     * Takes user type as a parameter. Creates an user according to type
     * @param user Type of the user
     */
    public tkArrayList(String user) {
        capacity = INITIAL_CAPACITY;
        if(user.equals("employee"))
            data = (E[]) new BranchEmployee[capacity];
        else if(user.equals("customer"))
            data = (E[]) new Customer[capacity];
        else if(user.equals("admin"))
            data = (E[]) new Administrator[capacity];

    }

    /**
     * Adds an user to the end of the list. Takes user information as parameter. If list is full, reallocates new memory for the list.
     * @param name Name of the user
     * @param surname Surname of the user
     * @param email Email of the user
     * @param password Password of the user
     * @return Returns true when adding is successful
     */
    public boolean add(String name, String surname, String email, String password) {
        if(size == capacity)
            reallocate();
        if(email.contains("employee"))
            data[size] = (E) new BranchEmployee(name, surname, email, password);
        else if(email.contains("customer"))
            data[size] = (E) new Customer(name, surname, email, password);
        else if(email.contains("admin"))
            data[size] = (E) new Administrator(name, surname, email, password);

        return true;
    }

    /**
     * Adds an user to the end of the list. Takes user itself as a parameter. If list is full, reallocates new memory for the list.
     * @param e User field
     * @return Returns true when adding is successful
     */
    public boolean add(E e){
        if(size == capacity)
            reallocate();
        data[size] = e;
        size++;
        return true;
    }

    /**
     * Adds user into given index. Reallocates new memory if list is full.
     * @param index Index to put user into
     * @param e User field
     */
    public void add(int index, E e) {
        if(index < 0  || index > size)
            throw new ArrayIndexOutOfBoundsException(index);
        if(size == capacity)
            reallocate();
        // shift elements
        for(int i=size; i > index; i--)
            data[i] = data[i-1];
        // insert e
        data[index] = e;
        // increase size
        size++;
    }

    /**
     * Returns the user with given index. Throws exception when index is invalid
     * @param index Shows which user to return
     * @return Selected user
     */
    public E get(int index) {
        if(index < 0 || index > size)
            throw new ArrayIndexOutOfBoundsException(index);
        return data[index];
    }

    /**
     * Changes user at the given index with given user. Throws exception when index is invalid
     * @param index Shows which user to change
     * @param e User to put into given index
     * @return Old user
     */
    public E set(int index, E e) {
        if(index < 0 || index > size)
            throw new ArrayIndexOutOfBoundsException(index);
        // getting old e
        E oldE = data[index];
        // inserting new one
        data[index] = e;
        // returning old one
        return oldE;
    }

    /**
     * Returns size of the arraylist
     * @return size of the arraylist
     */
    public int size() { return size; }

    /**
     * Returns index of given user. Throws exception when given user is invalid
     * @param e User field
     * @return index of the given user
     */
    public int indexOf(E e) {
        for(int i=0; i<size; i++)
            if(data[i] == e)
                return i;
        return -1;
    }

    /**
     * Removes the selected user from the list. Throws exception when index is invalid.
     * @param index User index to remove from list
     * @return Removed user
     */
    public E remove(int index) {
        if(index < 0 || index > size)
            throw new ArrayIndexOutOfBoundsException(index);
        // getting return value
        E returnValue = data[index];
        // shifting elements
        for(int i=index+1; i<size; i++)
            data[i-1] = data[i];
        // decreasing size
        size--;
        // returning return value
        return returnValue;
    }

    /**
     * Reallocates new memory for the arraylist
     */
    private void reallocate() {
        capacity = 2 * capacity;
        data = Arrays.copyOf(data, capacity);
    }

    public String toString() {
        String returnString = "[";

        for(int i=0; i<size; i++) {
            returnString += this.get(i);
            if(i == size-1)
                returnString += "]";
            else
                returnString += ", ";
        }
        return returnString;
    }
}
