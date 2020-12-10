using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Stoon : MonoBehaviour
{
    GameObject hugita; //オブジェクト入れる変数
    PlayerController PlayerControllerScript;　//スクリプト変数
    // Start is called before the first frame update
    void Start()
    {
        hugita = GameObject.Find("player"); 　//プレイヤー検索
        PlayerControllerScript = hugita.GetComponent<PlayerController>();　//player内のスクリプト[PlayerController]検索して入れている

    }

    // Update is called once per frame
    void OnCollisionEnter2D(Collision2D col)
    {

        if (col.gameObject.tag == "Player")
        {         
            PlayerControllerScript.fly();
        }
    }
     
}
