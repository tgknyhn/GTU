import java.io.*;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Scanner;

/**
 * Class that controls and contains general system of the E-shopping application.
 */
public class Application {
    // Products
    private final File productFile;
    private final File usersFile;
    // Users of the system ( ID & Password )
    private Hashtable<Integer, String> userIDs;


    // [ Constructor ]

    /**
     * Initializes Application object
     * @param productFile Holds products and all their information
     * @param usersFile Holds users and all their information
     * @throws IOException When "products.txt" or "users.txt" file doesn't exist
     */
    public Application(File productFile, File usersFile) throws IOException {
        this.productFile = productFile;
        this.usersFile   = usersFile;
        // clearing inside of the .txt files
        Writer temp  = new FileWriter(productFile, false);
        Writer temp2 = new FileWriter(usersFile, false);

        temp.close();
        temp2.close();

        userIDs    = new Hashtable<>();
    }

    // [ Public Methods ]

    /**
     * Turns the data in .csv file into more meaningful .txt file.
     * @param csvFile Path that contains .csv file
     * @throws IOException When given .csv path is invalid
     */
    public void csv_to_txt(File csvFile) throws IOException {
        Scanner readFromCSV = new Scanner(new FileReader(csvFile)).useDelimiter(";|\\n");
        Writer  writeToTXT  = new FileWriter(productFile);

        // skipping first line - optional
        readFromCSV.nextLine();

        String category;

        // translating data
        while(readFromCSV.hasNext()) {
            // ID
            writeToTXT.write("ID: " + readFromCSV.next() + "\n");
            // Product Name
            writeToTXT.write("Product Name: " + readFromCSV.next() + "\n");
            // Product Category Tree
            category = readFromCSV.next().replace("\"[\"\"", "").replace("\"\"]\"", "");
            writeToTXT.write("Product Category: " + category + "\n");
            // Price
            writeToTXT.write("Price: " + readFromCSV.next() + "\n");
            // Discounted Price
            writeToTXT.write("Discounted Price: " + readFromCSV.next() + "\n");
            // Description
            writeToTXT.write("Description: " + readFromCSV.next() + "\n");
            // Trader
            writeToTXT.write("Trader: " + readFromCSV.next() + "\n\n");
        }



        writeToTXT.close();
        readFromCSV.close();
    }

    /**
     * Searches through "users.txt" file to check if there is an user with given ID and password.
     * Returns true when there is an user with given ID and password.
     * If name section is not filled, then user is selected as Customer otherwise Trader.
     * @param ID ID of the User
     * @param password Password of the User
     * @return True when an user exist with same ID and password
     */
    public boolean login(int ID, String password) {
        int valid = userExist(ID, password);

        if(valid == 1) {
            System.out.println("User: " + ID + " successfully login to the system.");
            return true;
        }

        System.out.println("ERROR! Login unsuccessful. Invalid ID or password.");
        return false;
    }

    /**
     * Takes an user object as parameter and registers that user to the system if there isn't an User with given ID
     * @param newUser User that will be registered to the system
     * @throws IOException When file is empty
     */
    public void register(User newUser) throws IOException {
        int    ID       = newUser.getID();
        String password = newUser.getPassword();
        String name     = newUser.getName();

        if(userExist(ID, password) != -1) {
            System.out.println("ERROR! An user with same ID is already registered.");
            return;
        }
        else if(password.length() != 6) {
            System.out.println("ERROR! Password must contain 6 character. Your password has " + password.length());
            return;
        }
        else if(String.valueOf(ID).length() != 8) {
            System.out.println("ERROR! ID must be 8 digit. Your ID is " + String.valueOf(ID).length() + " digit.");
            return;
        }

        // writer
        Writer wUsersFile = new FileWriter(usersFile, true);

        // ID
        wUsersFile.write("ID: " + ID + "\n");
        // Name
        wUsersFile.write("Name: " + name + "\n");
        // Password
        wUsersFile.write("Password: " + password + "\n\n");

        // adding it into hashTable
        userIDs.put(ID, password);

        wUsersFile.close();

        System.out.println("User: " + ID + " is registered to the system.");
    }

    // [ Private Methods ]

    /**
     * Returns 1 when an user exist with same ID and password
     * Returns 0 when an user exist with only same ID (not password)
     * Return -1 when user doesn't exist
     * @return 1 (ID and password same), 0 (ID same), -1 (no match)
     */
    private int userExist(int ID, String password) {
        if(userIDs.get(ID) == null)
            return -1;
        else if(userIDs.get(ID).compareTo(password) == 0)
            return 1;
        else if(userIDs.containsKey(ID))
            return 0;
        else
            return -1;
    }
}
