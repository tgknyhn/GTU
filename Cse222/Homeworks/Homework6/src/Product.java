import java.awt.*;

/**
 * A class which holds all information a product contains
 */
public class Product {
    private String ID;
    private String productName;
    private String productCategory;
    private String description;
    private String trader;
    private int price;
    private int discountedPrice;



    // [Constructor]

    /**
     * Initializes Product object with given parameters
     * @param ID ID of the product
     * @param productName Name of the product
     * @param productCategory Category of the product
     * @param description Description of the product
     * @param trader Trader of the product
     * @param price Price of the product
     * @param discountedPrice Discounted price of the product
     */
    public Product(String ID, String productName, String productCategory, String description, String trader, int price, int discountedPrice) {
        if(ID.length() != 16) {
            System.out.println("ERROR! Product couldn't added. ID must have 16 characters");
            return;
        }

        this.ID = ID.toUpperCase();
        this.productName = productName;
        this.productCategory = productCategory;
        this.description = description;
        this.trader = trader;
        this.price = price;
        this.discountedPrice = discountedPrice;
    }


    // [Getters]

    /**
     * Returns ID of the product
     * @return ID of the product
     */
    public String getID() {
        return ID;
    }

    /**
     * Returns name of the product
     * @return Name of the product
     */
    public String getProductName() {
        return productName;
    }

    /**
     * Returns category of the product
     * @return Category of the product
     */
    public String getProductCategory() {
        return productCategory;
    }

    /**
     * Returns description of the product
     * @return Description of the product
     */
    public String getDescription() {
        return description;
    }

    /**
     * Returns trader of the product
     * @return Trader of the product
     */
    public String getTrader() {
        return trader;
    }

    /**
     * Returns price of the product
     * @return Price of the product
     */
    public int getPrice() {
        return price;
    }

    /**
     * Returns discounted price of the product
     * @return Discounted price of the product
     */
    public int getDiscountedPrice() {
        return discountedPrice;
    }

    /**
     * Returns discount percentage of the product
     * @return Discount percentage of the product
     */
    public float getDiscountPercentage() { return ((float)(price-discountedPrice) / price) * 100; }


    // [ Compare To ]

    /**
     * Compares given product's name with this object's name
     * @param p Product that will be compared
     * @return returns result of compareTo method for name string
     */
    public int compareName(Product p) {
        return Integer.compare(productName.compareTo(p.getProductName()), 0);
    }

    /**
     * Compares given product's price with this product's price
     * @param p Product that will be compared
     * @return 1 (given product is cheaper), 0 (same price), -1 (given product is more expensive)
     */
    public int comparePrice(Product p) {
        return Integer.compare(price, p.price);
    }

    /**
     * Compares given product's discount percentage with this product's
     * @param p Product that will be compared
     * @return 1 (given product has higher percentage), 0 (same percentage), -1 (given product has less percentage)
     */
    public int compareDiscountPercentage(Product p) {
        float percentage_1 =  ((float) (price - discountedPrice) / (price) ) * 100;
        float percentage_2 =  ((float) (p.price - p.discountedPrice) / (p.price) ) * 100;

        return Float.compare(percentage_1, percentage_2);
    }



    // To String

    @Override
    public String toString() {
        //noinspection StringBufferReplaceableByString
        StringBuilder string = new StringBuilder();

        // ID
        string.append("ID: ").append(getID()).append("\n");
        // Product Name
        string.append("Product Name: ").append(getProductName()).append("\n");
        // Product Category Tree
        string.append("Product Category: ").append(getProductCategory()).append("\n");
        // Price
        string.append("Price: ").append(getPrice()).append("\n");
        // Discounted Price
        string.append("Discounted Price: ").append(getDiscountedPrice()).append("\n");
        // Description
        string.append("Description: ").append(getDescription()).append("\n");
        // Trader
        string.append("Trader: ").append(getTrader()).append("\n");

        return string.toString();
    }
}
