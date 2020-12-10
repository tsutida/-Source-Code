using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LifeStockText : MonoBehaviour {

    int lifestock = 0;

    void Start()
    {
    }
    void Update()
    {
        //プレイヤーの残機数を変数lifestockで受け取る
        lifestock = 6;
        
        GetComponent<Text>().text = lifestock.ToString("0");
    }
}