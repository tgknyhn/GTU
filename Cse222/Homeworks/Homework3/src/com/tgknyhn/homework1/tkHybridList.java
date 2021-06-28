package com.tgknyhn.homework1;

import java.util.Arrays;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * HybridList for the furnitures in a furniture company automation system.
 * @param <E> Furniture type
 */
public class tkHybridList<E> {
    // Max Number
    private int MAX_NUMBER;

    // Linked List
    private hbLinkedList<hbArrayList<E>> furnitures;

    // Index
    private int index;

    /**
     * Initializes all data fields.
     */
    public tkHybridList() {
        MAX_NUMBER = 10;
        index = 0;
        furnitures = new hbLinkedList<>();
        furnitures.add(new hbArrayList<>());
    }

    /**
     * Adds a furniture to the end of the list. Takes furniture itself as a parameter. If list is full, creates new arraylist for the list.
     * @param e furniture data
     * @return Returns true when adding is successful
     */
    public boolean add(E e) {
        // creating new Arraylist if current one is full
        if(furnitures.get(index).size() == MAX_NUMBER) {
            furnitures.add(new hbArrayList<>());
            index++;
        }
        // adding element
        furnitures.get(index).add(e);

        return true;
    }

    /**
     * Returns the furniture with given index. Throws exception when index is invalid
     * @param index Shows which furniture to return
     * @return Selected furniture
     */
    public E get(int index) {
        if(index < 0 || index >= size())
            throw new ArrayIndexOutOfBoundsException(index);

        int firstIndex = index / MAX_NUMBER;
        int secondIndex = index % MAX_NUMBER;

        return furnitures.get(firstIndex).get(secondIndex);
    }

    /**
     * Returns size of the hybridlist
     * @return Size of the hybridlist
     */
    public int size() {
        return (index*MAX_NUMBER) + furnitures.get(index).size();
    }

    /**
     * Removes the selected furniture from the list. Throws exception when index is invalid.
     * @param index Furniture's location in the list
     * @return Removed furniture
     */
    public E remove(int index) {
        if(index < 0 || index >= size())
            throw new ArrayIndexOutOfBoundsException(index);
        if(size() == 0)
            throw new NullPointerException();

        int firstIndex = index / MAX_NUMBER;
        int secondIndex = index % MAX_NUMBER;

        E returnValue = furnitures.get(firstIndex).get(secondIndex);
        furnitures.get(firstIndex).set(secondIndex, null);

        if(furnitures.get(this.index).size() == 0 && this.index != 0) {
            furnitures.remove(this.index);
            this.index--;
        }

        return returnValue;
    }

    /**
     * Changes furniture at the given index with given furniture. Throws exception when index is invalid
     * @param index Shows which furniture to change
     * @param e furniture to put into given index
     */
    public void set(int index, E e) {
        if(index < 0 || index >= size())
            throw new ArrayIndexOutOfBoundsException(index);

        int firstIndex = index / MAX_NUMBER;
        int secondIndex = index % MAX_NUMBER;

        furnitures.get(firstIndex).set(secondIndex, e);
    }

    @Override
    public String toString() {
        String string = "";

        for(int i=0; i<=this.index; i++) {
            for(int j=0; j<furnitures.get(i).size(); j++) {
                string += furnitures.get(i).get(j) + ", ";
            }
        }

        return string;
    }
}

class hbLinkedList<E> {
    private Node<E> head;
    private Node<E> tail;
    private int size;

    public hbLinkedList() {
        head = null;
        tail = null;
        size = 0;
    }

    public E get(int index) {
        return listIterator(index).next();
    }

    public E getFirst() {
        return listIterator(0).next();
    }

    public E getLast() { return listIterator(size-1).next(); }

    public int size() {
        return size;
    }

    public E set(int index, E e) {
        myListIterator iterator = listIterator(index);

        E returnValue = iterator.nextItem.data;

        iterator.nextItem.data = e;

        return returnValue;
    }

    public int indexOf(E e) {
        myListIterator iterator = listIterator(0);
        while(iterator.hasNext()) {
            if(iterator.next() == e)
                return iterator.index;
        }
        return -1;
    }

    public E remove(int index) { return listIterator(index).remove(0); }

    public boolean add(E e) {
        addLast(e);
        return true;
    }

    public void add(int index, E e) {
        listIterator(index).add(e);
    }

    public void addFirst(E e) {
        listIterator(0).add(e);
    }

    public void addLast(E e) {
        listIterator(size).add(e);
    }

    public myListIterator listIterator(int index) {
        return new myListIterator(index);
    }

    private class myListIterator implements ListIterator<E> {
        private Node<E> nextItem;
        private Node<E> lastItemReturned;
        private int index = 0;

        private myListIterator(int i) {
            if(i < 0 || i > size)
                throw new ArrayIndexOutOfBoundsException(index);

            if(i == size)
                index = size;
            else {
                nextItem = head;
                for(index = 0; index < i; index++)
                    nextItem = nextItem.next;
            }

            lastItemReturned = nextItem;

        }

        @Override
        public boolean hasNext() { return nextItem != null; }

        @Override
        public E next() {
            if(!hasNext())
                throw new NoSuchElementException();
            lastItemReturned = nextItem;
            nextItem = nextItem.next;
            index++;
            return lastItemReturned.data;
        }

        @Override
        public boolean hasPrevious() { return (nextItem == null && size > 0) || nextItem.prev != null; }

        @Override
        public E previous() {
            if(!hasPrevious())
                throw new NoSuchElementException();
            if(nextItem == null)
                nextItem = tail;
            else
                nextItem = nextItem.prev;
            lastItemReturned = nextItem;
            index--;
            return lastItemReturned.data;
        }

        @Override
        public int nextIndex() { return index+1; }

        @Override
        public int previousIndex() { return index-1; }

        @Override
        public void remove() {
            if(!hasPrevious())
                head = nextItem.next;
            else if(hasNext())
                nextItem.prev.next = nextItem.next;

            if(nextItem.next == null)
                tail = nextItem.prev;
            else
                nextItem.next.prev = nextItem.prev;

            size--;
            index--;
        }

        public E remove(int ignoreMe) {
            remove();
            return nextItem.data;
        }

        @Override
        public void set(E e) { /* Empty */ }

        @Override
        public void add(E e) {
            if(head == null) {
                head = new Node<>(e);
                tail = head;
            }
            else if(nextItem == head) {
                Node<E> newNode = new Node<>(e);
                newNode.next = nextItem;
                nextItem.prev = newNode;
                head = newNode;
            }
            else if(nextItem == null) {
                Node<E> newNode = new Node<>(e);
                tail.next = newNode;
                newNode.prev = tail;
                tail = newNode;
            }
            else {
                Node<E> newNode = new Node<>(e);
                newNode.prev = nextItem.prev;
                nextItem.prev.next = newNode;
                newNode.next = nextItem;
                nextItem.prev = newNode;
            }
            size++;
            index++;
            lastItemReturned = null;
        }
    }

    private class Node<E> {
        private E data;
        private Node<E> next = null;
        private Node<E> prev = null;

        private Node(E data) { this.data = data; }
    }

    @Override
    public String toString() {
        String elements = "";

        Iterator<E> iterator = listIterator(0);

        if(iterator.hasNext())
            elements = "[" + iterator.next();

        while(iterator.hasNext())
            elements += "," + iterator.next();

        elements += "]";

        return elements;
    }
}

class hbArrayList<E> {
    private static final int INITIAL_CAPACITY = 10;
    private E[] data;
    private int size = 0;
    private int capacity = 0;

    public hbArrayList() {
        capacity = INITIAL_CAPACITY;
        data = (E[]) new Object[capacity];
    }

    public boolean add(E e){
        if(size == capacity)
            reallocate();
        data[size] = e;
        size++;
        return true;
    }

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

    public E get(int index) {
        if(index < 0 || index > size)
            throw new ArrayIndexOutOfBoundsException(index);
        return data[index];
    }

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

    public int size() { return size; }

    public int indexOf(E e) {
        for(int i=0; i<size; i++)
            if(data[i] == e)
                return i;
        return -1;
    }

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
