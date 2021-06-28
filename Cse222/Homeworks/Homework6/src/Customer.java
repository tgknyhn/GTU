import java.io.*;
import java.util.*;

/**
 * Customer class of the E-shopping application. Implements User interface
 */
public class Customer implements User{
    private final File productFile;    // product file
    private final File orderFile;

    // User Information
    private int    ID;
    private String password;
    private String name;

    private final ArrayList<String> commandsList;    // contains available commands for the Trader
    private CategoryTree            categories;




    // [Constructor]

    /**
     * Initializes Customer object with given parameters
     * @param ID ID of the Customer
     * @param password Password of the Customer
     * @param productFile Product file contains the current product list which the customer will search in.
     * @param orderFile When customer orders a product, it will be added to this file.
     */
    public Customer(int ID, String password, File productFile, File orderFile) {
        this.ID = ID;
        this.password = password;
        this.productFile = productFile;
        this.orderFile = orderFile;
        // categories
        categories = new CategoryTree();
        // Commands
        commandsList = new ArrayList<>();
        commandsList.add("1) Search products by using product name");
        commandsList.add("2) Search products by using trader name");
        commandsList.add("3) Exit");
    }





    // [Getters]

    @Override
    public int getID() {
        return ID;
    }

    @Override
    public String getPassword() {
        return password;
    }

    @Override
    public String getName() {
        return name;
    }





    // [Setters]

    @Override
    public void setID(int ID) {
        this.ID = ID;
    }

    @Override
    public void setPassword(String password) {
        this.password = password;
    }

    @Override
    public void setName(String name) {
        this.name = name;
    }





    // [Command Methods]

    @Override
    public void exeCommand(int command) {
        // scanner for userInput
        Scanner scanner = new Scanner(System.in);

        try {
            //noinspection EnhancedSwitchMigration
            switch(command) {
                case 1:
                    System.out.println("Enter the product name: ");
                    exeSecCommand(getProductsByName(scanner.nextLine()));
                    break;
                case 2:
                    System.out.print("Enter the trader: ");
                    exeSecCommand(getProductsByTrader(scanner.nextLine()));
                    break;
                case 3:
                    System.out.println("\nReturning to main menu...\n");
                    break;
                default:
                    System.out.println("ERROR! Invalid input");
                    break;
            }
        } catch (FileNotFoundException exception) {
            System.out.println("ERROR! \"products.txt\" file couldn't open.");
        }
    }

    /**
     * Takes searched product list as parameter and let's customer to sort, filter or order products.
     * @param products Product list that will be used in order, sort and filter section.
     */
    private void exeSecCommand(LinkedList<Product> products) {
        // scanner for userInput
        Scanner scanner = new Scanner(System.in);
        String userInput = "";
        boolean leave = false;
        int basePrice, topPrice, product;
        String baseCategory, topCategory;

        // categories
        categories = new CategoryTree();
        for(int i=0; i<products.size(); i++)
            categories.addAll(products.get(i).getProductCategory());

        while(!leave) {
            for(int i=0; i<products.size(); i++)
                System.out.println((i+1) + ") Product Name: " + products.get(i).getProductName() + "\t| Price: " + products.get(i).getPrice() + "\t| Discount Percentage: " + products.get(i).getDiscountPercentage());

            System.out.println("******************************");
            System.out.println("1) Give order");
            System.out.println("2) Sort searched products by their name");
            System.out.println("3) Sort searched products by their price");
            System.out.println("4) Sort searched products by their discount percentage");
            System.out.println("5) Category filter");
            System.out.println("6) Price filter");
            System.out.println("7) Exit");
            System.out.print  ("Enter: ");

            userInput = scanner.nextLine();

            try {
                switch(userInput) {
                    case "1":
                        System.out.print("Product index: ");
                        product = Integer.parseInt(scanner.nextLine());
                        giveOrder(products.get(product-1).getID());
                        leave = true;
                        break;
                    case "2":
                        sortByName(products);
                        break;
                    case "3":
                        sortByPrice(products);
                        break;
                    case "4":
                        sortByDiscountPercentage(products);
                        break;
                    case "5":
                        System.out.print("Base category: ");
                        baseCategory = scanner.nextLine();
                        System.out.print("Top category: ");
                        topCategory = scanner.nextLine();
                        categoryFilter(baseCategory, topCategory, products);
                        break;
                    case "6":
                        System.out.print("Base price: ");
                        basePrice = Integer.parseInt(scanner.nextLine());
                        System.out.print("Top price: ");
                        topPrice = Integer.parseInt(scanner.nextLine());
                        priceFilter(basePrice, topPrice, products);
                        break;
                    case "7":
                        System.out.println("Returning to customer");
                        leave = true;
                        break;
                    default:
                        System.out.println("ERROR! Invalid input.");
                        break;
                }

            } catch (IOException exception) {
                System.out.println("ERROR! \"product.txt\" file couldn't open.");
                leave = true;
            }
        }

    }

    @Override
    public void printCommandList() {
        System.out.println("Command List for the Customer");
        System.out.println("*****************************");
        for(String command : commandsList)
            System.out.println(command);

        System.out.print("Enter: ");
        Scanner scanner = new Scanner(System.in);

        try {
            exeCommand(Integer.parseInt(scanner.nextLine()));
        } catch (NumberFormatException exception) {
            System.out.println("ERROR! Invalid input.");
        }
    }




    // [Customer's unique methods]

    /**
     * Takes product ID as parameter and finds that product in "products.txt" file.
     * If product exist in the file than creates an order that product and writes the order in "orders.txt" file.
     * @param productID ID of the product that will be ordered
     * @throws IOException When opening file is unsuccessful
     */
    public void giveOrder(String productID) throws IOException {
        // writer
        Writer writer = new FileWriter(orderFile, true);

        if(!doesContain(productID)) {
            System.out.println("ERROR! Item you are trying to order doesn't exist.");
            return;
        }
        else
            System.out.println("You ordered the product. ID: " + productID + ". (You can check current status of your product in \"orders.txt\" file)");

        // Product ID
        writer.write("Product ID: " + productID + "\n");
        // Customer ID
        writer.write("Customer ID: " + getID() + "\n");
        // State of the product
        writer.write("Current Status: Waiting for trader's answer...\n\n");


        writer.flush();
        writer.close();
    }


    // [Product Lister]

    /**
     * Returns all products that contains given parameter in their name and description as Product object with LinkedList data type.
     * @param name Name of the product
     * @return All products that contains given name parameter
     * @throws FileNotFoundException When file doesn't exist
     */
    public LinkedList<Product> getProductsByName(String name) throws FileNotFoundException {
        // reader
        Scanner reader = new Scanner(new FileReader(productFile));
        // product list
        LinkedList<Product> products = new LinkedList<>();

        String ID, productName, productCategory, price_str, discountedPrice_str, description, trader;
        int price, discountedPrice;

        while(reader.hasNext()) {
            // getting product data
            ID                  = reader.nextLine().replace("ID: ", "");
            productName         = reader.nextLine().replace("Product Name: ", "");
            productCategory     = reader.nextLine().replace("Product Category: ", "");
            price_str           = reader.nextLine().replace("Price: ", "");
            discountedPrice_str = reader.nextLine().replace("Discounted Price: ", "");
            description         = reader.nextLine().replace("Description: ", "");
            trader              = reader.nextLine().replace("Trader: ", "");
            // skipping empty line
            reader.nextLine();

            // checking if this is the product that we are searching
            if(!productName.contains(name) && !description.contains(name))
                continue;

            // string to int
            price           = Integer.parseInt(price_str);
            discountedPrice = Integer.parseInt(discountedPrice_str);

            products.add(new Product(ID, productName, productCategory, description, trader, price, discountedPrice));
        }

        reader.close();

        return products;
    }

    /**
     * Returns all products that contains given parameter in their Trader section as Product object with LinkedList data type.
     * @param name Name of the trader
     * @return All products that contains given Trader
     * @throws FileNotFoundException When file doesn't exist
     */
    public LinkedList<Product> getProductsByTrader(String name) throws FileNotFoundException {
        // reader
        Scanner reader = new Scanner(new FileReader(productFile));
        // product list
        LinkedList<Product> products = new LinkedList<>();

        String ID, productName, productCategory, price_str, discountedPrice_str, description, trader;
        int price, discountedPrice;

        while(reader.hasNext()) {
            // getting product data
            ID                  = reader.nextLine().replace("ID: ", "");
            productName         = reader.nextLine().replace("Product Name: ", "");
            productCategory     = reader.nextLine().replace("Product Category: ", "");
            price_str           = reader.nextLine().replace("Price: ", "");
            discountedPrice_str = reader.nextLine().replace("Discounted Price: ", "");
            description         = reader.nextLine().replace("Description: ", "");
            trader              = reader.nextLine().replace("Trader: ", "");
            // skipping empty line
            reader.nextLine();

            // checking if this is the product that we are searching
            if(!trader.equals(name))
                continue;

            // string to int
            price           = Integer.parseInt(price_str);
            discountedPrice = Integer.parseInt(discountedPrice_str);

            products.add(new Product(ID, productName, productCategory, description, trader, price, discountedPrice));
        }

        reader.close();

        return products;
    }



    // [Sorting]

    /**
     * Sorts given LinkedList Product object list according to their names. a -> z
     * @param products Product list that will be sorted
     */
    public void sortByName(LinkedList<Product> products) { MergeSort.sort(products); }

    /**
     * Sorts given LinkedList Product object list according to their price. 1 -> 100
     * @param products Product list that will be sorted
     */
    public void sortByPrice(LinkedList<Product> products) { InsertionSort.sort(products); }

    /**
     * Sorts given LinkedList Product object list according to their Discount percentage. 1 -> 100
     * @param products Product list that will be sorted
     */
    public void sortByDiscountPercentage(LinkedList<Product> products) { SelectionSort.sort(products); }



    // [Filters]

    /**
     * Filters the product list according to base and top category that is given as parameter.
     * @param baseCategory Lowest node in category tree
     * @param topCategory Highest node in category tree
     * @param products Product list that will be filtered
     */
    public void categoryFilter(String baseCategory, String topCategory, LinkedList<Product> products) {
        // adding all categories
        for(int i=0; i<products.size(); i++)
            categories.addAll(products.get(i).getProductCategory());

        ArrayList<String> validCategory = new ArrayList<>();
        StringBuilder validCategories = new StringBuilder();

        validCategory.add(baseCategory);
        baseCategory = categories.parentCategory(baseCategory);

        while(!baseCategory.equals(topCategory)) {
            validCategory.add(baseCategory);
            baseCategory = categories.parentCategory(baseCategory);
        }
        validCategory.add(baseCategory);

        int index = validCategory.size() - 1;

        validCategories.append(validCategory.get(index));

        for(int i=validCategory.size()-2; i>=0; i--)
            validCategories.append(" >> ").append(validCategory.get(i));

        System.out.println(validCategories.toString());

        for(int i=0; i<products.size(); i++) {
            if(!products.get(i).getProductCategory().equals(validCategories.toString())) {
                products.remove(i);
                i--;
            }
        }
    }

    /**
     * Filters the product list according to base and top price that is given as parameter
     * @param basePrice Lowest price that will be listed
     * @param topPrice Highest price that will be listed
     * @param products Product list that will be filtered
     */
    public void priceFilter(int basePrice, int topPrice, LinkedList<Product> products) {
        for(int i=0; i<products.size(); i++)
            if(products.get(i).getPrice() < basePrice || products.get(i).getPrice() > topPrice) {
                products.remove(i);
                i--;
            }
    }

    // [Helper Methods]

    // returns true when given product ID exist in the product file
    private boolean doesContain(String productID) throws IOException {
        // Reading from text file
        Scanner productRead = new Scanner(new FileReader(productFile)).useDelimiter("ID: |Product Name: ");

        while(productRead.hasNext()) {
            // current ID
            String currID = productRead.next();
            // empty line
            productRead.next();

            // return true if item is present in the text file
            if(currID.contains(productID)) {
                productRead.close();
                return true;
            }
        }
        // closing the reader
        productRead.close();

        return false;
    }
}


