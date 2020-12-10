using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Score : MonoBehaviour
{
    int ScoreData = 0;
    int ScoreTime = 0;
    void Start()
    {
    }
    void Update()
    {
        //プレイ中少しずつスコアが上がる
        //if (Game_Status == PLAY)
        //{
            ScoreTime++;
            if (ScoreTime > 255)
            {
                ScoreData += 10;
                ScoreTime = 0;
            }
        //}
        GetComponent<Text>().text = ScoreData.ToString("0");
    }

    //敵を倒したときなどスコアが上昇する時このScoreUp関数をとして呼び出す　　上昇するスコア量を引数とする
    void ScoreUp(int UpScore)
    {
        ScoreData += UpScore;
    }
}
