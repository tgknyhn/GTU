using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;
using UnityEngine.Profiling;

public class ButtonHandlerNoScale : MonoBehaviour
{
    string filePath1 = "pointSet3.txt";
    string filePath2 = "pointSet4.txt";

    public Transform[] points1;
    public Transform[] points2;
    Vector3[] points;
    Vector4[] refPoints;
    KabschAlgorithm solver = new KabschAlgorithm();

    int pointSize1 = 5, pointSize2 = 3;

    // Start is called before the first frame update
    void Start()
    {
        pointSize1 = fromTxtToTransformArray(filePath1, points1, new Color(0, 200, 0), new Vector3(0.4f, 1f, 0.4f));
        pointSize2 = fromTxtToTransformArray(filePath2, points2, new Color(200, 0, 0), new Vector3(1f, 0.4f, 0.4f));

        points = new Vector3[pointSize1];
        refPoints = new Vector4[pointSize2];

        for (int i = 0; i < pointSize1; i++)
        {
            points[i] = points1[i].position;
        }
    }

    // Update is called once per frame
    void Update()
    {

    }

    public int fromTxtToTransformArray(string filePath, Transform[] points, Color color, Vector3 newScale)
    {
        // Reading the file
        using (Stream stream = File.Open(filePath, FileMode.Open))
        using (TextReader sr = new StreamReader(stream, Encoding.UTF8))
        {
            string line = sr.ReadLine();
            int lineCount = Int32.Parse(line);

            for (int i = 0; i < lineCount; i++)
            {
                line = sr.ReadLine();
                string[] arr = line.Split(' ');
                points[i].position = new UnityEngine.Vector3(float.Parse(arr[0]), float.Parse(arr[1]), float.Parse(arr[2]));
                points[i].GetComponent<Renderer>().material.color = color;
                points[i].localScale = newScale;
            }

            return lineCount;
        }
    }

    public void createSphere(Transform point, Color color)
    {
        GameObject sphere = GameObject.CreatePrimitive(PrimitiveType.Sphere);

        sphere.GetComponent<Renderer>().material.color = color;

        sphere.transform.position = new UnityEngine.Vector3(point.position.x, point.position.y, point.position.z);
    }

    public void OnClick()
    {
        for (int i = 0; i < pointSize2; i++)
        {
            refPoints[i] = new Vector4(points2[i].position.x, points2[i].position.y, points2[i].position.z, points2[i].localScale.x);
        }

        Matrix4x4 kabschTransform = solver.SolveKabsch(points, refPoints);

        for (int i = 0; i < pointSize1; i++)
        {
            points1[i].position = kabschTransform.MultiplyPoint3x4(points[i]);
            //points1[i].position = new Vector3(10, 10, 10);
        }
    }
}

//Kabsch Implementation-----------------------------------------------------------
public class KabschAlgorithm
{
    Vector3[] QuatBasis = new Vector3[3];
    Vector3[] DataCovariance = new Vector3[3];
    Quaternion OptimalRotation = Quaternion.identity;
    public float scaleRatio = 1f;
    public Matrix4x4 SolveKabsch(Vector3[] points1, Vector4[] refPoints, bool solveRotation = true, bool solveScale = false)
    {
        if (points1.Length != refPoints.Length) { return Matrix4x4.identity; }

        //Calculate the centroid offset and construct the centroid-shifted point matrices
        Vector3 inCentroid = Vector3.zero; Vector3 refCentroid = Vector3.zero;
        float inTotal = 0f, refTotal = 0f;
        for (int i = 0; i < points1.Length; i++)
        {
            inCentroid += new Vector3(points1[i].x, points1[i].y, points1[i].z) * refPoints[i].w;
            inTotal += refPoints[i].w;
            refCentroid += new Vector3(refPoints[i].x, refPoints[i].y, refPoints[i].z) * refPoints[i].w;
            refTotal += refPoints[i].w;
        }
        inCentroid /= inTotal;
        refCentroid /= refTotal;

        //Calculate the scale ratio
        if (solveScale)
        {
            float inScale = 0f, refScale = 0f;
            for (int i = 0; i < points1.Length; i++)
            {
                inScale += (new Vector3(points1[i].x, points1[i].y, points1[i].z) - inCentroid).magnitude;
                refScale += (new Vector3(refPoints[i].x, refPoints[i].y, refPoints[i].z) - refCentroid).magnitude;
            }
            scaleRatio = (refScale / inScale);
        }

        //Calculate the 3x3 covariance matrix, and the optimal rotation
        if (solveRotation)
        {
            Profiler.BeginSample("Solve Optimal Rotation");
            extractRotation(TransposeMultSubtract(points1, refPoints, inCentroid, refCentroid, DataCovariance), ref OptimalRotation);
            Profiler.EndSample();
        }

        return Matrix4x4.TRS(refCentroid, Quaternion.identity, Vector3.one * scaleRatio) *
               Matrix4x4.TRS(Vector3.zero, OptimalRotation, Vector3.one) *
               Matrix4x4.TRS(-inCentroid, Quaternion.identity, Vector3.one);
    }

    //https://animation.rwth-aachen.de/media/papers/2016-MIG-StableRotation.pdf
    //Iteratively apply torque to the basis using Cross products (in place of SVD)
    void extractRotation(Vector3[] A, ref Quaternion q)
    {
        for (int iter = 0; iter < 9; iter++)
        {
            Profiler.BeginSample("Iterate Quaternion");
            q.FillMatrixFromQuaternion(ref QuatBasis);
            Vector3 omega = (Vector3.Cross(QuatBasis[0], A[0]) +
                             Vector3.Cross(QuatBasis[1], A[1]) +
                             Vector3.Cross(QuatBasis[2], A[2])) *
             (1f / Mathf.Abs(Vector3.Dot(QuatBasis[0], A[0]) +
                             Vector3.Dot(QuatBasis[1], A[1]) +
                             Vector3.Dot(QuatBasis[2], A[2]) + 0.000000001f));

            float w = omega.magnitude;
            if (w < 0.000000001f)
                break;
            q = Quaternion.AngleAxis(w * Mathf.Rad2Deg, omega / w) * q;
            q = Quaternion.Lerp(q, q, 0f); //Normalizes the Quaternion; critical for error suppression
            Profiler.EndSample();
        }
    }

    //Calculate Covariance Matrices --------------------------------------------------
    public static Vector3[] TransposeMultSubtract(Vector3[] vec1, Vector4[] vec2, Vector3 vec1Centroid, Vector3 vec2Centroid, Vector3[] covariance)
    {
        Profiler.BeginSample("Calculate Covariance Matrix");
        for (int i = 0; i < 3; i++)
        { //i is the row in this matrix
            covariance[i] = Vector3.zero;
        }

        for (int k = 0; k < vec1.Length; k++)
        {//k is the column in this matrix
            Vector3 left = (vec1[k] - vec1Centroid) * vec2[k].w;
            Vector3 right = (new Vector3(vec2[k].x, vec2[k].y, vec2[k].z) - vec2Centroid) * Mathf.Abs(vec2[k].w);

            covariance[0][0] += left[0] * right[0];
            covariance[1][0] += left[1] * right[0];
            covariance[2][0] += left[2] * right[0];
            covariance[0][1] += left[0] * right[1];
            covariance[1][1] += left[1] * right[1];
            covariance[2][1] += left[2] * right[1];
            covariance[0][2] += left[0] * right[2];
            covariance[1][2] += left[1] * right[2];
            covariance[2][2] += left[2] * right[2];
        }

        Profiler.EndSample();
        return covariance;
    }
    public static Vector3[] TransposeMultSubtract(Vector3[] vec1, Vector3[] vec2, ref Vector3[] covariance)
    {
        for (int i = 0; i < 3; i++) covariance[i] = Vector3.zero;

        for (int k = 0; k < vec1.Length; k++)
        {//k is the column in this matrix
            Vector3 left = vec1[k];
            Vector3 right = vec2[k];

            covariance[0][0] += left[0] * right[0];
            covariance[1][0] += left[1] * right[0];
            covariance[2][0] += left[2] * right[0];
            covariance[0][1] += left[0] * right[1];
            covariance[1][1] += left[1] * right[1];
            covariance[2][1] += left[2] * right[1];
            covariance[0][2] += left[0] * right[2];
            covariance[1][2] += left[1] * right[2];
            covariance[2][2] += left[2] * right[2];
        }
        return covariance;
    }
}

public static class FromMatrixExtension
{
    public static Vector3 GetVector3(this Matrix4x4 m) { return m.GetColumn(3); }
    public static Quaternion GetQuaternion(this Matrix4x4 m)
    {
        if (m.GetColumn(2) == m.GetColumn(1)) { return Quaternion.identity; }
        return Quaternion.LookRotation(m.GetColumn(2), m.GetColumn(1));
    }
    public static void FillMatrixFromQuaternion(this Quaternion q, ref Vector3[] covariance)
    {
        covariance[0] = q * Vector3.right;
        covariance[1] = q * Vector3.up;
        covariance[2] = q * Vector3.forward;
    }
    public static Matrix4x4 Lerp(Matrix4x4 a, Matrix4x4 b, float alpha)
    {
        return Matrix4x4.TRS(Vector3.Lerp(a.GetVector3(), b.GetVector3(), alpha), Quaternion.Slerp(a.GetQuaternion(), b.GetQuaternion(), alpha), Vector3.one);
    }
}
