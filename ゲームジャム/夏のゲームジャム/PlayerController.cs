using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    [SerializeField] private float groundspeed = 0.1f;
    [SerializeField] private float speed = 0.02f;
    [SerializeField] public GameObject badpoint1;
    [SerializeField] public GameObject badpoint2;
    [SerializeField] public GameObject goodpoint;
    [SerializeField] bool startflg = true;
    [SerializeField] bool flingflg = false;
    [SerializeField] bool slide = false;
    [SerializeField] bool bad1 = false;
    [SerializeField] bool bad2 = false;
    [SerializeField] bool good = false;
  
    Vector3 offset;
    Vector3 target;
    float deg;
    float TimeCount = 5;



    IEnumerator Playerfling()
    {
        float b = Mathf.Tan(deg * Mathf.Deg2Rad);
        float a = (target.y - b * target.x) / (target.x * target.x);

        for (float x = 0; x <= this.target.x; x += 0.3f)
        {
            float y = a * x * x + b * x;
            transform.position = new Vector3(x, y, 0) + offset;
            yield return null;
        }
    }
    public void SetTarget(Vector3 target, float deg)
    {
        this.offset = transform.position;
        this.target = target - this.offset;
        this.deg = deg;

        StartCoroutine("Playerfling");
    }

    void Start()
    {    
    }

    private void Update()
    {
        TimeCount -= Time.deltaTime;


        if (TimeCount <= 0)
        {
            if (startflg == true)
            {
                this.gameObject.transform.Translate(groundspeed, 0, 0);
            }
            if (slide == true)
            {
                this.gameObject.transform.Translate(speed, 0, 0);
            }

        }
        if (flingflg == true)
        {
            // ブロックに向かって40度の角度で射出
            if (bad1 == true)
            {
                SetTarget(badpoint1.transform.position, 70);
                flingflg = false;
            }
            if (bad2 == true)
            {
                SetTarget(badpoint2.transform.position, 70);
                flingflg = false;
            }
            if (good == true)
            {
                SetTarget(goodpoint.transform.position, 70);
                flingflg = false;

            }
        }
    }
    public void fly()
    {
        flingflg = true;
        startflg = false;      
    }
    public void gooded()
    {
        good = true;
        bad1 = false;
        slide = true;
    }

    public void bad1ed()
    {
        bad1 = true;
        slide = true;
    }

    public void bad2ed()
    {
        bad2 = true;
        bad1 = false;
        good = false;

    }
}
