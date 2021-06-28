package com.tgknyhn.homework1;

/**
 * Implements Furniture interface for an Office Cabinet. It contains all properties for an office cabinet  
 */
public class OfficeCabinet extends Furniture {

    // Office Cabinet(OCB) properties
    private final static String  nameOCB   = "Office Cabinet";
    private final static int     priceOCB  = 300;
    /**
     * Possible colors for an office cabinet
     */
    public  final static Color[] colorsOCB = {Color.DEFAULT};
    /**
     * Possible models for an office cabinet
     */
    public  final static Model[] modelsOCB = {Model.v1, Model.v2, Model.v3, Model.v4, Model.v5, Model.v6, Model.v7, Model.v8, Model.v9, Model.v10, Model.v11, Model.v12};
    /**
     * Preferred constructor for Office Cabinet class. Includes all properties
     * @param model Model of the Office Cabinet
     */
    public OfficeCabinet(Model model) { super(nameOCB, Color.DEFAULT, colorsOCB, model, modelsOCB, priceOCB); }

}
