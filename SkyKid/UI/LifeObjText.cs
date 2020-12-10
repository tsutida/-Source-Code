using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class LifeObjText : MonoBehaviour {

    [SerializeField] private GameObject life;
    [SerializeField] private GameObject lifeobj1;
    [SerializeField] private GameObject lifeobj2;
    [SerializeField] private GameObject lifeobj3;
    [SerializeField] private GameObject lifeobj4;

    int lifestock =0;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        //プレイヤーの残機数を変数lifestockで受け取る
        lifestock = 6;

        //残機1～4の場合はアイコン数で表現する
        if (lifestock >= 1)
        {
            Instantiate(lifeobj1, new Vector2(-3.6f, 4.79f), transform.rotation);
            Destroy(this.gameObject);	//自分を消去する
        }
        if (lifestock >= 2 && lifestock < 5)
        {
            Instantiate(lifeobj2, new Vector2(-3.3f, 4.79f), transform.rotation);
            Destroy(this.gameObject);	//自分を消去する
        }
        if (lifestock >= 3 && lifestock < 5)
        {
            Instantiate(lifeobj3, new Vector2(-3.0f, 4.79f), transform.rotation);
            Destroy(this.gameObject);	//自分を消去する
        }
        if (lifestock >= 4 && lifestock < 5)
        {
            Instantiate(lifeobj4, new Vector2(-2.7f, 4.79f), transform.rotation);
            Destroy(this.gameObject);	//自分を消去する
        }

        //残機数5以降はテキスト表示
        if (lifestock >= 5)
        {
            life.SetActive(true);
            Destroy(this.gameObject);	//自分を消去する
        }
        else
        {
            life.SetActive(false);
            Destroy(this.gameObject);	//自分を消去する
        }
    }
}
