package com.tgknyhn.homework1;

import java.util.Iterator;
import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * LinkedList for the branches in a furniture company automation system.
 * @param <E> Branch Type
 */
public class tkLinkedList<E> {
    private Node<E> head;
    private Node<E> tail;
    private int size;

    /**
     * Initializes data fields.
     */
    public tkLinkedList() {
        head = null;
        tail = null;
        size = 0;
    }

    /**
     * Returns the branch with given index. Throws exception when index is invalid
     * @param index Shows which branch to return
     * @return Selected branch
     */
    public E get(int index) {
        return listIterator(index).next();
    }

    /**
     * Returns first branch of the list.
     * @return First branch of list
     */
    public E getFirst() {
        return listIterator(0).next();
    }

    /**
     * Returns last branch of the list.
     * @return Last branch of list
     */
    public E getLast() { return listIterator(size-1).next(); }

    /**
     * Returns size of the linkedlist
     * @return size of the linkedlist
     */
    public int size() {
        return size;
    }

    /**
     * Changes branch at the given index with given branch. Throws exception when index is invalid
     * @param index Shows which branch to change
     * @param e Branch to put into given index
     * @return Old branch
     */
    public E set(int index, E e) {
        myListIterator iterator = listIterator(index);

        E returnValue = iterator.nextItem.data;

        iterator.nextItem.data = e;

        return returnValue;
    }

    /**
     * Returns index of given branch. Throws exception when given user is invalid
     * @param e Branch data field
     * @return index of the given Branch
     */
    public int indexOf(E e) {
        myListIterator iterator = listIterator(0);
        while(iterator.hasNext()) {
            if(iterator.next() == e)
                return iterator.index;
        }
        return -1;
    }

    /**
     * Removes the selected branch from the list. Throws exception when index is invalid.
     * @param index Branch index to remove from list
     * @return Removed branch
     */
    public E remove(int index) { return listIterator(index).remove(0); }

    /**
     * Adds a branch to the end of the list. Takes branch's itself as a parameter.
     * @param e Branch data field
     * @return Returns true when adding is successful
     */
    public boolean add(E e) {
        addLast(e);
        return true;
    }

    /**
     * Adds branch into given index.
     * @param index Index to put branch into
     * @param e Branch data field
     */
    public void add(int index, E e) {
        listIterator(index).add(e);
    }

    /**
     * Adds branch into first node.
     * @param e Branch data field
     */
    public void addFirst(E e) {
        listIterator(0).add(e);
    }

    /**
     * Adds branch into last node.
     * @param e Branch data field
     */
    public void addLast(E e) {
        listIterator(size).add(e);
    }

    /**
     * Returns myListIterator for linkedlist. Iterator starts from given index.
     * @param index Index to start from
     * @return myListIterator
     */
    public myListIterator listIterator(int index) {
        return new myListIterator(index);
    }

    /**
     * Iterator class for linkedlist. Implements ListIterator interface.
     */
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

    /**
     * Holds nodes for linkedlist
     * @param <E> Data type that node holds
     */
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
