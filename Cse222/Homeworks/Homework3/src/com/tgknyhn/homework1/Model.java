package com.tgknyhn.homework1;

/**
 * Model enumerator for a furniture. Contains 12 models.
 */
public enum Model {
    v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;

    /**
     * Returns the equivalent model of the given string parameter as enum
     * @param name Name of the model
     * @return Model as enum
     */
    static public Model getModel(String name) {
        Model model;

        switch (name) {
            case "v2":
                model = Model.v2;
                break;
            case "v3":
                model = Model.v3;
                break;
            case "v4":
                model = Model.v4;
                break;
            case "v5":
                model = Model.v5;
                break;
            case "v6":
                model = Model.v6;
                break;
            case "v7":
                model = Model.v7;
                break;
            case "v8":
                model = Model.v8;
                break;
            case "v9":
                model = Model.v9;
                break;
            case "v10":
                model = Model.v10;
                break;
            case "v11":
                model = Model.v11;
                break;
            case "v12":
                model = Model.v12;
                break;
            default:
                model = Model.v1;
                break;
        }
        return model;
    }


}
