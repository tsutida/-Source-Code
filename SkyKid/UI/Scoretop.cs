using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Scoretop : MonoBehaviour
{
    int TopScoreData = 0;
    void Start()
    {
    }
    void Update()
    {
        //最高得点を変数TopScoreDataで受け取る
        TopScoreData = 1234560;
        GetComponent<Text>().text = TopScoreData.ToString("0");
    }
}