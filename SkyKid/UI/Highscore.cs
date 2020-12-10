using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Highscore : MonoBehaviour {
    [SerializeField] private Text scoreText;  
    [SerializeField] private Text highScoreText;// ハイスコアを表示する

    // スコア
    public static int score;

    // ハイスコア
    private static int highScore;

    private string key = "highScore";
    // Use this for initialization
    void Start () {
        highScore = PlayerPrefs.GetInt(key, 0);//保存しておいたハイスコアをキーで呼び出し取得し保存されていなければ0になる
        highScoreText.text = "HighScore: " + highScore.ToString();  //ハイスコアを表示
        //PlayerPrefs.DeleteAll(); //リセットコマンド(有効にすると現在ハイスコアリセット
    }

    // Update is called once per frame
    void Update () {
        if (score > highScore)
        {

            highScore = score;
            //ハイスコア更新

            PlayerPrefs.SetInt(key, highScore);
            //ハイスコアを保存

            highScoreText.text = "HighScore: " + highScore.ToString();
            //ハイスコアを表示
        }

        // スコア・ハイスコアを表示する
        scoreText.text = score.ToString();
        highScoreText.text =highScore.ToString();
    }

    // ゲーム開始前の状態に戻す
    private void Initialize()
    {
        // スコアを0に戻す
        score = 0;

        // ハイスコアを取得する。保存されてなければ0を取得する。
        highScore = PlayerPrefs.GetInt(key, 0);
       

    }

    // ポイントの追加
    public void AddPoint(int point)
    {
        score = score + point;
    }

}
