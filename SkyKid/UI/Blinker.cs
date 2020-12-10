using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Blinker : MonoBehaviour
{

    public float interval = 0.7f;



    void Start()
    {

        StartCoroutine("Blink");
    }
    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {

            interval = 0.1f;
        }
    }
  

IEnumerator Blink()
    {
     
            while (true)
            {
                var renderComponent = GetComponent<Renderer>();
                renderComponent.enabled = !renderComponent.enabled;
                yield return new WaitForSeconds(interval);
            }
        }      
}