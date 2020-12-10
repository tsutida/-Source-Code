using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Stage : MonoBehaviour {
    
    void Start()
    {
    }
    void Update()
    {
        //ステージ数を変数stageで受け取る
        int stage = 1;

        GetComponent<Text>().text = stage.ToString("1");
    }
}
