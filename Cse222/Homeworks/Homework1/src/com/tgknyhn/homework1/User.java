package com.tgknyhn.homework1;

/**
 * An interface for Users. It contains all necessary methods and fields for communication with a company.
 */
public interface User {

    /**
     * Returns name of the User
     * @return Name of the User
     */
    String getName();

    /**
     * Returns surname of the User
     * @return Surname of the User
     */
    String getSurname();

    /**
     * Returns E-mail of the User
     * @return E-mail of the User
     */
    String getEmail();

    /**
     * Returns password of the User
     * @return Password of the User
     */
    String getPassword();

    /**
     * Returns address of the user
     * @return Address of the user
     */
    default String getAddress() { return null; }

    /**
     * Returns phone number of the user
     * @return Phone number of the user
     */
    default long getPhoneNumber() { return 0; }

    /**
     * Returns the ID of customer
     * @return ID of the customer
     */
    default int getCustomerID() { return 0; }

    /**
     * Returns full name of the previous order
     * @param index Shows which previous order to return
     * @return Full name of the previous order
     */
    default String getOrder(int index) { return "empty"; }

    /**
     * Returns number of orders that user made
     * @return Number of orders that user made
     */
    default int getOrderCount() { return 0; }

    /**
     * Changes name with given parameter
     * @param name Name of the User
     */
    void setName(String name);

    /**
     * Changes surname with given parameter
     * @param surname Surname of the User
     */
    void setSurname(String surname);

    /**
     * Changes email with given parameter
     * @param email Email of the User
     */
    void setEmail(String email);

    /**
     * Changes user password with given parameter
     * @param password Password of the User
     */
    void setPassword(String password);

    /**
     * Executed when customer buys a product. Sets bought item's information as previous order to reach them later.
     * Also increases the order count by one
     * @param productName Name of the product which bought by the customer
     * @param productModel Model of the product which bought by the customer
     * @param productColor Color of the product which bought by the customer
     * @throws ArrayIndexOutOfBoundsException After 5 order, customer reaches limit. Any execution of this function after 5 order will throw this exception.
     */
    default void setOrder(String productName, Model productModel, Color productColor) throws ArrayIndexOutOfBoundsException { /* empty */ }

    /**
     * Sets customer's order count as the given parameter
     * @param count Number of orders that customer made
     */
    default void setOrderCount(int count) { /* empty */ }


    /**
     * Sets customer's address as the given parameter.
     * @param address Address of the customer
     */
    default public void setAddress(String address) { /* empty */ }

    /**
     * Sets customer's phone number as the given parameter.
     * @param phoneNumber Phone number of the customer.
     */
    default public void setPhoneNumber(long phoneNumber) { /* empty */ }


    /**
     * Shows and executes available commands for the User
     * @param company company which User login to
     */
    void commands(Company company, FurnitureBranch[] branches, User[] employees);




}
