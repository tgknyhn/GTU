using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CameraController : MonoBehaviour
{
    public float camSpeed = -5f;
    private float x;
    private float y;
    private Vector3 rotateValue;

    public Camera mainCamera;
    public float speed = 5.0f;
    public float sensitivity = 5.0f;
    public float zoomSensitivity = 5.0f;
    public float minFOV = 15f;
    public float maxFOV = 90f;

    void Update()
    {
        x = Input.GetAxis("Mouse X");
        y = Input.GetAxis("Mouse Y");

        rotateValue = new Vector3(y, x * -1, 0);

        transform.eulerAngles = transform.eulerAngles - rotateValue;
        transform.eulerAngles +=  rotateValue * camSpeed * Time.deltaTime;

        // Use the left and right arrow keys to move the camera horizontally
        float horizontal = Input.GetAxis("Horizontal");
        transform.position += new Vector3(horizontal, 0, 0) * speed * Time.deltaTime;

        // Use the up and down arrow keys to move the camera vertically
        float vertical = Input.GetAxis("Vertical");
        transform.position += new Vector3(0, 0, vertical) * speed * Time.deltaTime;

        // Use the shift key to move the camera down and the space key to move it up
        if (Input.GetKey(KeyCode.LeftShift))
        {
            transform.position += new Vector3(0, -1, 0) * speed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.Space))
        {
            transform.position += new Vector3(0, 1, 0) * speed * Time.deltaTime;
        }

        // Use the mouse scroll wheel to zoom in and out by changing the field of view
        float zoom = Input.GetAxis("Mouse ScrollWheel");
        mainCamera.fieldOfView += zoom * zoomSensitivity;
        mainCamera.fieldOfView = Mathf.Clamp(mainCamera.fieldOfView, minFOV, maxFOV);
    }
}

/*
public Camera mainCamera;
    public RawImage outputImage;
    public float speed = 5.0f;
    public float sensitivity = 5.0f;
    public float zoomSensitivity = 5.0f;
    public float minFOV = 15f;
    public float maxFOV = 90f;

    void Update()
    {
        // Use the left and right arrow keys to move the camera horizontally
        float horizontal = Input.GetAxis("Horizontal");
        transform.position += new Vector3(horizontal, 0, 0) * speed * Time.deltaTime;

        // Use the up and down arrow keys to move the camera vertically
        float vertical = Input.GetAxis("Vertical");
        transform.position += new Vector3(0, 0, vertical) * speed * Time.deltaTime;

        // Use the mouse to rotate the camera horizontally and vertically
        float mouseX = Input.GetAxis("Mouse X");
        float mouseY = Input.GetAxis("Mouse Y");
        transform.Rotate(0, mouseX * sensitivity, 0);
        //transform.Rotate(-mouseY * sensitivity, 0, 0);

        // Use the shift key to move the camera down and the space key to move it up
        if (Input.GetKey(KeyCode.LeftShift))
        {
            transform.position += new Vector3(0, -1, 0) * speed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.Space))
        {
            transform.position += new Vector3(0, 1, 0) * speed * Time.deltaTime;
        }

        // Use the mouse scroll wheel to zoom in and out by changing the field of view
        float zoom = Input.GetAxis("Mouse ScrollWheel");
        mainCamera.fieldOfView += zoom * zoomSensitivity;
        mainCamera.fieldOfView = Mathf.Clamp(mainCamera.fieldOfView, minFOV, maxFOV);
    }

*/