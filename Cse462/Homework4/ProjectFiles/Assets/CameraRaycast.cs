using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CameraRaycast : MonoBehaviour
{
    public Camera _camera;
    public RawImage rawImage;
    private Texture2D texture;
    public List<Light> lights;  
    
    public GameObject blackhole;

    private Ray[,] pixelRays = new Ray[640,480];
    private bool[,] pixelHit = new bool[640,480];

    void Start()
    {
        // Initialize the texture with the specified dimensions
        texture = new Texture2D(640, 480);

        // Update the raw image with the new texture
        rawImage.texture = texture;

        // Shoot the ray 640x480 times
        for (int x = 0; x < 640; x++)
        {
            for (int y = 0; y < 480; y++)
            {
                // Calculate the position of the current pixel
                Vector3 pixelPos = new Vector3(x, y, 0);

                // Create a ray from the camera through the current pixel
                pixelRays[x,y] = _camera.ScreenPointToRay(pixelPos);
                pixelHit[x,y] = false;
            }
        }
    }

    void Update()
    {
        // Check if the user has pressed the left mouse button
        if (Input.GetMouseButtonDown(0))
        {
            // Shoot the ray 640x480 times
            for (int x = 0; x < 640; x++)
            {
                for (int y = 0; y < 480; y++)
                {
                    // Calculate the position of the current pixel
                    Vector3 pixelPos = new Vector3(x, y, 0);

                    // Create a ray from the camera through the current pixel
                    pixelRays[x,y] = _camera.ScreenPointToRay(pixelPos);
                    pixelHit[x,y] = false;

                }
            }

            // Update the texture with the newly set pixels
            texture.Apply();
        }
        
        // Shoot the ray 640x480 times
        for (int x = 0; x < 640; x++)
        {
            for (int y = 0; y < 480; y++)
            {
                if(pixelHit[x,y] == true)
                {
                    continue;
                }
                // Perform raycast with the pixel ray
                RaycastHit hit;

                bool hitSomething = false;

                if(blackhole != null) {
                    hitSomething = Physics.Raycast(pixelRays[x,y], out hit, 1);
                } else {
                    hitSomething = Physics.Raycast(pixelRays[x,y], out hit);
                }

                if(hitSomething)
                {
                    pixelHit[x,y] = true;
                    // If the ray hit an object, set the pixel's color to the object's color
                    Color pixelColor = hit.transform.GetComponent<MeshRenderer>().material.color;

                    // Check if the object is in shadow by looping through all the lights
                    foreach(Light light in lights)
                    {
                        Ray shadowRay = new Ray(hit.point, -light.transform.forward);
                        RaycastHit shadowHit;
                        if(Physics.Raycast(shadowRay, out shadowHit))
                        {
                            // If the shadow ray hit something, that means the object is in shadow
                            // Adjust the pixel color based on the shadow intensity
                            pixelColor = Color.Lerp(pixelColor, Color.black, .3f);
                        }
                    }

                    texture.SetPixel(x,y, pixelColor);
                }
                else
                {
                    if(blackhole != null) {
                        pixelRays[x,y].origin = pixelRays[x,y].origin + (pixelRays[x,y].direction.normalized);
                        Vector3 direction = (blackhole.transform.position - pixelRays[x,y].origin);
                        Vector3 normalizedDirection = ((direction.normalized) + (20 * pixelRays[x,y].direction)).normalized;

                        pixelRays[x,y].direction = normalizedDirection;
                    }
                    //pixelRays.origin = pixelRays.origin + pixelRays.direction;
                    //pixelRays.direction = blackhole.transform.position - pixelRays.direction;
                    texture.SetPixel(x, y, Color.black);
                }
            }
        }
        // Update the texture with the newly set pixels
        texture.Apply();
    }

}


/*
public class CameraRaycast : MonoBehaviour
{
    public Camera _camera;
    public RawImage rawImage;
    private Texture2D texture;
    public List<Light> lights;  
    
   void Start()
    {
        // Initialize the texture with the specified dimensions
        texture = new Texture2D(640, 480);

        // Update the raw image with the new texture
        rawImage.texture = texture;
    }

    void Update()
    {
        // Check if the user has pressed the left mouse button
        if (Input.GetMouseButtonDown(0))
        {
            // Shoot the ray 640x480 times
            for (int x = 0; x < 640; x++)
            {
                for (int y = 0; y < 480; y++)
                {
                    // Calculate the position of the current pixel
                    Vector3 pixelPos = new Vector3(x, y, 0);

                    // Create a ray from the camera through the current pixel
                    Ray pixelRay = _camera.ScreenPointToRay(pixelPos);

                    // Perform raycast with the pixel ray
                    RaycastHit hit;
                    if(Physics.Raycast(pixelRay, out hit))
                    {
                        // If the ray hit an object, set the pixel's color to the object's color
                        Color pixelColor = hit.transform.GetComponent<MeshRenderer>().material.color;

                        // Check if the object is in shadow by looping through all the lights
                        foreach(Light light in lights)
                        {
                            Ray shadowRay = new Ray(hit.point, -light.transform.forward);
                            RaycastHit shadowHit;
                            if(Physics.Raycast(shadowRay, out shadowHit))
                            {
                                // If the shadow ray hit something, that means the object is in shadow
                                // Adjust the pixel color based on the shadow intensity
                                pixelColor = Color.Lerp(pixelColor, Color.black, .3f);
                                //pixelColor *= 1 + (light.shadowStrength-0.95f);
                                //Debug.Log(light.shadowStrength);
                            }
                        }

                        texture.SetPixel(x,y, pixelColor);
                    }
                    else
                    {
                        // If the ray didn't hit an object, set the pixel's color to black
                        texture.SetPixel(x, y, Color.black);
                    }
                }
            }

            // Update the texture with the newly set pixels
            texture.Apply();

        }
    }

}
*/