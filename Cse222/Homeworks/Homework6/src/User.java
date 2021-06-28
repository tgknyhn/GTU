/**
 * A general interface for the users of e-shopping application
 */
public interface User {

    /**
     * Returns ID of the User
     * @return ID of the User
     */
    int getID();

    /**
     * Returns password of the User
     * @return Password of the User
     */
    String getPassword();

    /**
     * Returns name of the User
     * @return Name of the User
     */
    String getName();

    /**
     * Changes ID of the User with given parameter
     * @param ID New ID of the User
     */
    void setID(int ID);

    /**
     * Changes password of the User with given parameter
     * @param password New password of the User
     */
    void setPassword(String password);

    /**
     * Changes name of the User with given parameter
     * @param name New name of the User
     */
    void setName(String name);

    /**
     * Executes given command for the User
     * Command list can be accessed by using printCommandList() method
     * @param command Selected command that will be executed
     */
    void exeCommand(int command);

    /**
     * Prints available commands for the user to the console
     * After seeing commands user can execute any available command by using exeCommand
     */
    void printCommandList();


    // ************* [DEFAULT METHODS] *************

    /**
     * Returns true if given parameters is the same with this user's information
     * @param ID ID of searched person
     * @param password Password of searched person
     * @return True when parameters is same with this user's information
     */
    default boolean isSamePerson(int ID, String password) {
        return (getID() == ID) && (getPassword().equals(password));
    }

}
