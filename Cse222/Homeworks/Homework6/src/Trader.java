import java.io.*;
import java.util.*;

/**
 * Trader class of E-shopping application. Implements User interface.
 */
public class Trader implements User {
    private final File productFile;
    private final File orderFile;

    // User Information
    private int    ID;
    private String password;
    private String name;

    private final ArrayList<String> commandsList;    // contains available commands for the Trader





    // [Constructor]

    /**
     * Initializes Trader object with given parameters.
     * @param ID ID of the trader
     * @param password Password of the trader
     * @param name Name of the trader
     * @param productFile Contains product list that contains all products
     * @param orderFile Contains all orders given by customers
     */
    public Trader(int ID, String password, String name, File productFile, File orderFile) {
        // User Info
        this.ID          = ID;
        this.password    = password;
        this.name        = name;
        this.productFile = productFile;
        this.orderFile   = orderFile;
        // Commands
        commandsList = new ArrayList<>();
        commandsList.add("1) Add Product");
        commandsList.add("2) Remove Product");
        commandsList.add("3) Edit Product");
        commandsList.add("4) Handle an order");
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
        // reader for handling
        Scanner reader;
        try {
            reader = new Scanner(new FileReader(orderFile));
        } catch (FileNotFoundException exception) {
            System.out.println("ERROR! \"orders.txt\" file couldn't open.");
            return;
        }
        // scanner for userInput
        Scanner scanner = new Scanner(System.in);
        // products
        Product product, editedProduct;
        String productID;

        try {
            //noinspection EnhancedSwitchMigration
            switch (command) {
                case 1:
                    product = createProduct();
                    addProduct(product);
                    break;
                case 2:
                    System.out.print("ID of the product you want to remove: ");
                    removeProduct(scanner.next());
                    break;
                case 3:
                    System.out.print("ID of the product you want to edit: ");
                    productID = scanner.next();

                    System.out.println("\nEdit the product");
                    System.out.println("****************");
                    editedProduct = createProduct();

                    editProduct(productID, editedProduct);
                    break;
                case 4:
                    int index = 1;
                    String tempPID, tempCID, cancel;
                    ArrayList<String> pID = new ArrayList<>();
                    ArrayList<String> cID = new ArrayList<>();

                    while(reader.hasNext()) {
                        // product ID
                        tempPID = reader.nextLine().replace("Product ID: ", "");
                        // customer ID
                        tempCID = reader.nextLine().replace("Customer ID: ", "");
                        // status
                        reader.nextLine();

                        if(isThereOrderForID(tempPID)) {
                            System.out.println(index + ") Product ID: " + pID + " | Customer ID: " + cID + "\n");
                            pID.add(tempPID);
                            cID.add(tempCID);
                            index++;
                        }
                    }
                    System.out.print("Enter: ");
                    index = Integer.parseInt(scanner.nextLine());
                    System.out.print("cancel or meet the order: ");
                    cancel = scanner.nextLine();

                    if(cancel.equals("cancel"))
                        handleOrder(pID.get(index-1), true);
                    else if(cancel.equals("meet"))
                        handleOrder(pID.get(index-1), false);
                    else
                        System.out.println("ERROR! Invalid input.");
                    break;
                default:
                    System.out.println("ERROR! Invalid command.");
                    break;
            }
        } catch (IOException I) {
            System.out.println("EXCEPTION ! exeCommand() method, invalid IO value.");
        }

    }

    @Override
    public void printCommandList() {
        System.out.println("Command List for the Trader");
        System.out.println("***************************");
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





    // [Trader's unique methods]

    /**
     * Adds given product to the product file
     * @param product New product that will be added
     * @throws IOException When productFile is not valid
     */
    public void addProduct(Product product) throws IOException {
        // if given product doesn't belong to this trader, return without adding.
        if(!product.getTrader().equals(this.name)) {
            System.out.println("ERROR! Given product doesn't belong to this trader.");
            return;
        }

        // Writing to text file
        Writer productWrite = new FileWriter(productFile, true);

        // return without adding if item is present
        if(doesContain(product.getID())) {
            System.out.println("The item you are trying to add already exists.");
            return;
        }
        else
            System.out.println("Item with ID: " + product.getID() + " is added to the product list");

        // Adding item to the file
        productWrite.write(product.toString() + "\n");
        // Closing the writer
        productWrite.close();
    }

    /**
     * Removes given product from the product file
     * @param productID ID of the product that will be removed from the product file
     * @throws IOException When productFile is not valid
     */
    public void removeProduct(String productID) throws IOException {
        // if given product doesn't belong to this trader, return without removing.
        if(doesContain(productID)) {
            System.out.println("ERROR! Product list doesn't contain this item for this trader.");
            return;
        }

        // Creating a temp product file
        File productFile_tmp  = new File("tempProduct.txt");
        // reference of product file
        File productFile_curr = productFile;

        // opening it with writing properties
        Writer productWrite = new FileWriter(productFile_tmp, false);
        // Reading from text file
        Scanner productRead = new Scanner(new FileReader(productFile_curr)).useDelimiter("\\n\\n");

        while(productRead.hasNext()) {
            // getting current line as String
            String currProduct = productRead.next();

            // if deleteID is found skip that product
            if(currProduct.contains(productID))
                continue;

            // writing into temp product file
            productWrite.write(currProduct + "\n\n");
        }
        // closing reading and writing objects
        productRead.close();
        productWrite.close();
        // removing & renaming files
        if(productFile_curr.delete())
            if(productFile_tmp.renameTo(productFile))
                System.out.println("The item with the ID: " + productID + " removed from the product list.");
    }

    /**
     * Changes information of the product with the given ID
     * @param oldProductID ID of the product that will be edited
     * @param newProduct New data for the product
     * @throws IOException When product file is not valid
     */
    public void editProduct(String oldProductID, Product newProduct) throws IOException {
        // if given product doesn't belong to this trader, return without adding.
        if(!newProduct.getTrader().equals(this.name)) {
            System.out.println("ERROR! Given products don't belong to this trader.");
            return;
        }
        else if(doesContain(oldProductID)) {
            System.out.println("ERROR! Product list doesn't contain this item.");
            return;
        }
        else
            System.out.println("Product with ID: " + newProduct.getID() + " is edited.");

        // removing oldProduct
        removeProduct(oldProductID);
        // adding edited one
        addProduct(newProduct);
    }

    /**
     * If the given order is belong to this trader's product, then handles the product by canceling or meeting the request.
     * @param orderID ID of the product that is ordered
     * @param cancel Trader's decision on order
     * @throws IOException When file doesn't exist
     */
    public void handleOrder(String orderID, boolean cancel) throws IOException {
        if(!isThereOrderForID(orderID)) {
            System.out.println("ERROR! There is no order with given product ID.");
            return;
        }

        // Temp orderFile
        File temp = new File("tempOrders.txt");
        File curr = orderFile;
        // Reader
        Scanner reader = new Scanner(new FileReader(curr));
        // Writer
        Writer writer = new FileWriter(temp,false);
        // Temp order info holders
        String pID = "", cID, status, eof;

        Queue<String> orders = new ArrayDeque<>();

        // reading orders
        while(reader.hasNext()) {
            pID = reader.nextLine() + "\n";     // product ID
            cID = reader.nextLine() + "\n";     // customer ID
            status = reader.nextLine() + "\n";  // Current Status
            eof = reader.nextLine() + "\n";     // end of line

            orders.add(pID + cID + status + eof);
        }

        // writing into temp file
        while(!orders.isEmpty()) {
            // getting order
            String order = orders.poll();

            // changing information if this is the order we look for
            if(order.contains(orderID) && !order.contains("Canceled") && !order.contains("Sold!")) {
                if(cancel)
                    order = order.replace("Waiting for trader's answer...", "Canceled!");
                else
                    order = order.replace("Waiting for trader's answer...", "Sold!");
            }

            writer.write(order);
        }

        reader.close();
        writer.close();

        if(curr.delete())
            if(temp.renameTo(orderFile))
                System.out.println("Product with ID: " + pID + " is handled");

    }



    // [Trader's private methods]

    private boolean isThereOrderForID(String productID) throws IOException {
        // reader
        Scanner reader = new Scanner(new FileReader(orderFile));
        // order information
        String pID, status;
        Product product;

        while(reader.hasNext()) {
            // product ID
            pID = reader.nextLine();
            // customer ID
            reader.nextLine();
            // current status
            status = reader.nextLine();
            // end of line
            reader.nextLine();

            if(!status.contains("Waiting for trader's answer"))
                continue;

            if(pID.contains(productID)) {
                product = getProductByID(productID);
                if(product != null)
                    if(product.getTrader().equals(name)) {
                        reader.close();
                        return true;
                    }
            }
        }
        reader.close();
        return false;
    }

    // returns true when given product ID exist in the product file
    private boolean doesContain(String productID) throws IOException {
        // Reading from text file
        Scanner productRead = new Scanner(new FileReader(productFile)).useDelimiter("ID: |Product Name: ");

        while(productRead.hasNext()) {
            // current ID
            String currID = productRead.next();

            // if the item doesn't contain this trader, pass the item
            if(!productRead.next().contains(this.name))
                continue;

            // return true if item is present in the text file
            if(currID.equals(productID)) {
                productRead.close();
                return true;
            }
        }
        // closing the reader
        productRead.close();

        return false;
    }

    // finds product with the given ID and returns it
    private Product getProductByID(String productID) throws IOException {
        // Reading from text file
        Scanner productRead = new Scanner(new FileReader(productFile));

        String ID, productName, productCategory, price_str, discountedPrice_str, description, trader;
        int price, discountedPrice;

        while(productRead.hasNext()) {
            // getting product data
            ID                  = productRead.nextLine();
            productName         = productRead.nextLine();
            productCategory     = productRead.nextLine();
            price_str           = productRead.nextLine();
            discountedPrice_str = productRead.nextLine();
            description         = productRead.nextLine();
            trader              = productRead.nextLine();
            // skipping empty line
            productRead.nextLine();

            // checking if this is the product that we are searching
            if(!ID.contains(productID))
                continue;

            // removing unnecessary parts
            ID              = ID.replace("ID: ", "");
            productName     = productName.replace("Product Name: ", "");
            productCategory = productCategory.replace("Product Category: ", "");
            price           = Integer.parseInt(price_str.replace("Price: ", ""));
            discountedPrice = Integer.parseInt(discountedPrice_str.replace("Discounted Price: ", ""));
            description     = description.replace("Description: ", "");
            trader          = trader.replace("Trader: ", "");

            productRead.close();

            return new Product(ID, productName, productCategory, description, trader, price, discountedPrice);
        }

        productRead.close();
        // item doesn't exist if method reaches here
        System.out.println("ERROR! Item with ID: " + productID + " doesn't exist");
        return null;
    }

    // Takes user input to create a new product and returns it as an Product object
    private Product createProduct(){
        // return value
        Product product;
        // scanner for user input
        Scanner scanner = new Scanner(System.in);
        // product information
        String productID;
        String productName;
        String productCategory;
        String description;
        String trader = this.name;
        int price;
        int discountedPrice;

        // user input
        System.out.print("Product ID: ");
        productID = scanner.nextLine().toUpperCase();
        System.out.print("Product Name: ");
        productName = scanner.nextLine();
        System.out.print("Product Category: ");
        productCategory = scanner.nextLine();
        System.out.print("Product Description: ");
        description = scanner.nextLine();
        System.out.print("Product Price: ");
        price = Integer.parseInt(scanner.nextLine());
        System.out.print("Product Discounted Price: ");
        discountedPrice = Integer.parseInt(scanner.nextLine());

        // creating new product with given informations
        product = new Product(productID, productName, productCategory, description, trader, price, discountedPrice);

        // returning created product
        return product;
    }
}
