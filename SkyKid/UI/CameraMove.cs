using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMove : MonoBehaviour
{
    [SerializeField, Range(-1.0f, 1.0f)]
    private float speed = 0.006f;
    bool flg;
    void Start()
    {
        StartCoroutine("Next");

    }

    void Update()
    {
        if (flg == true) { this.transform.position -= new Vector3(speed, 0.0f, 0.0f); }
    }

    IEnumerator Next()
    {
       
        yield return new WaitForSeconds(3);
        flg = true;
    }
}
