package com.tgknyhn.homework1;

/**
 * Implements Furniture interface for a Bookcase. It contains all properties for a bookcase 
 */
public class Bookcase extends Furniture {

    // Bookcase(BC) properties
    private final static String  nameBC   = "Bookcase";
    private final static int     priceBC  = 250;
    
    /**
     * Possible models for a bookcase
     */
    public  final static Model[] modelsBC = {Model.v1, Model.v2, Model.v3, Model.v4, Model.v5, Model.v6, Model.v7, Model.v8, Model.v9, Model.v10, Model.v11, Model.v12};
    /**
     * Possible colors for a bookcase
     */
    public  final static Color[] colorsBC = {Color.DEFAULT};
    /**
     * Preferred constructor for Bookcase class. Includes all properties
     * @param model Model of the Bookcase
     */
    public Bookcase(Model model) { super(nameBC, Color.DEFAULT, colorsBC, model, modelsBC, priceBC); }

}
