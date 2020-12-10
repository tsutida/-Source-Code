using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleMove : MonoBehaviour
{
    [SerializeField, Range(-1.0f, 1.0f)]
    private float speed = 0.06f;
    bool flg = true;
    void Start()
    {

    }

    void Update()
    {
        if (flg == true){
            this.transform.position -= new Vector3(speed, 0.0f, 0.0f);
        };
        Invoke("Finish", 2.0f);
    }
    void Finish()
    {
        flg = false;

    }
}