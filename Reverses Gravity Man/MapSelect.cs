using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

public class MapSelect : MonoBehaviour {


    public AudioClip choiceSound;
    AudioSource audioSource;
    bool flg = false;
    float time=0.0f;
    // Use this for initialization
    void Start () {
        audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (flg)
        {
            time += Time.deltaTime;
            if(time > 1)
            {
                // メインシーンへ移動
                SceneManager.LoadScene("Masa");
            }
        }
    }
    public void OnClick()
    {
        Map.mapcnt = 1;
        //音楽の再生
        audioSource.PlayOneShot(choiceSound);
        flg = true;
      
    }
    public void OnClick2()
    {
        Map.mapcnt = 2;
        //音楽の再生
        audioSource.PlayOneShot(choiceSound);
        flg = true;

    }
    public void OnClick3()
    {
        Map.mapcnt = 3;
        //音楽の再生
        audioSource.PlayOneShot(choiceSound);
        flg = true;

    }
   /* public void OnClick4()
    {
        Map.mapcnt = 3;
        //音楽の再生
        audioSource.PlayOneShot(choiceSound);
        flg = true;

    }
    public void OnClick5()
    {
        Map.mapcnt = 3;
        //音楽の再生
        audioSource.PlayOneShot(choiceSound);
        flg = true;

    }*/
}
