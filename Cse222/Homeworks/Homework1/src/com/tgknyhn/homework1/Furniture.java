package com.tgknyhn.homework1;

/**
 * Represents Furnitures in a Furniture Branch. Every furniture have it's own type, color, model and name.
 */
public abstract class Furniture {

    // Name
    private final String name;
    // Color
    private final Color   color;
    // Model
    private final Model   model;

    /**
     * Holds possible colors for a furniture
     */
    public  final Color[] colors;
    /**
     * Holds possible models for a furniture
     */
    public  final Model[] models;
    
    // Price
    private final int price;

    /**
     * Preferred constructor for Furniture class. Includes all properties
     * @param name Name of the furniture
     * @param color Color of the furniture
     * @param colors Available colors for the furniture
     * @param model Model of the furniture
     * @param models Available models for the furniture
     * @param price Price of the furniture
     */
    public Furniture(String name, Color color, Color[] colors, Model model, Model[] models, int price) {
        this.name = name;
        this.color = color;
        this.colors = colors;
        this.model = model;
        this.models = models;
        this.price = price;
    }

    /**
     * Returns name of the furniture
     * @return Name of the furniture
     */
    public String getName() { return name; }

    /**
     * Returns color of the furniture
     * @return Color of the furniture
     */
    public Color getColor() { return color; }

    /**
     * Returns model of the furniture
     * @return Model of the furniture
     */
    public Model getModel() { return model; }

    /**
     * Returns price of the furniture
     * @return Price of the furniture
     */
    public int getPrice() { return price; }

    @Override
    public String toString() {
        String string = getName();
        string += " (Color: " + getColor() + ")";
        string += " (Model: " + getModel() + ")";
        string += " (Price: " + getPrice() + "$)";

        return string;
    }
}
