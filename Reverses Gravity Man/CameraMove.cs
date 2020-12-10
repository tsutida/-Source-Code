using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMove : MonoBehaviour
{
    [SerializeField]
    private float speed;
    bool flg;


    void Start()
    {
        speed = 5;
        StartCoroutine("Next");
        
    }

    void Update()
    {
        if (flg == true ) { this.transform.position -= new Vector3(-speed, 0.0f, 0.0f) * Time.deltaTime; }

     
    }

    IEnumerator Next()
    {
       
        yield return new WaitForSeconds(3);
        flg = true;
    }
}
