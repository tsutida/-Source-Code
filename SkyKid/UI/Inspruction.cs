using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Inspruction : MonoBehaviour {
    [SerializeField] private GameObject Redy; //Redyテキスト
    [SerializeField] private GameObject Takeoff; //TakeOffテキスト
    [SerializeField] private GameObject HalfwayPoint; //HalfwayPointテキスト
    [SerializeField] private GameObject GetBomb; //GetBomテキスト
    [SerializeField] private GameObject Land;
    [SerializeField] private GameObject Clear;
    // Use this for initialization
    void Start () {
        StartCoroutine("First");
    }
	
	// Update is called once per frame
	void Update () {
		
	}
    IEnumerator First()
    {
        yield return new WaitForSeconds(3);
        Redy.SetActive(false);
        Takeoff.SetActive(true);
        yield return new WaitForSeconds(3);
        Takeoff.SetActive(false);
        yield return new WaitForSeconds(5);
        HalfwayPoint.SetActive(true);
        yield return new WaitForSeconds(2);
        HalfwayPoint.SetActive(false);
        GetBomb.SetActive(true);
        yield return new WaitForSeconds(1);
        GetBomb.SetActive(false);
        yield return new WaitForSeconds(8);
        Land.SetActive(true);
        yield return new WaitForSeconds(2);
        Clear.SetActive(true);
        yield return new WaitForSeconds(2);
        Land.SetActive(false);
    }
}
