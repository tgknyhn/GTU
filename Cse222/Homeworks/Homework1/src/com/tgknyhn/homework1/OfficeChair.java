package com.tgknyhn.homework1;

/**
 * Implements Furniture interface for an Office Chair. It contains all properties for an office chair  
 */
public class OfficeChair extends Furniture {

    // Office Chair(OC) properties
    private final static String nameOC   = "Office Chair";
    private final static int    priceOC  = 100;
    /**
     * Possible models for an office chair
     */
    public final static Model[] modelsOC = {Model.v1, Model.v2, Model.v3, Model.v4, Model.v5, Model.v6, Model.v7};
    /**
     * Possible colors for an office chair
     */
    public final static Color[] colorsOC = {Color.BLACK, Color.RED, Color.WHITE, Color.PINK, Color.BLUE};

    /**
     * Preferred constructor for OfficeChair class. Includes all properties
     * @param color Color of the office chair
     * @param model Model of the office chair
     */
    public OfficeChair(Color color, Model model) { super(nameOC, color, colorsOC, model, modelsOC, priceOC); }

}
