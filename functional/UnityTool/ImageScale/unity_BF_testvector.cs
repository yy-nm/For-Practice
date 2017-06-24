
using UnityEngine;
using UnityEditor;

using System.Collections;
using System;
using System.IO;

using Mard.Tools.ImageScale;

[ExecuteInEditMode]
public class unity_BF_testvector : MonoBehaviour {

	public Texture2D tex32;
	public Texture2D tex24;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnEnable() 
	{
		TestVector ();
	}

	public void TestVector()
	{
		if (null != tex32)
			Test32 ();
		if (null != tex24)
			Test24 ();

		AssetDatabase.Refresh ();
	}

	public void Test24() {

		int wpow = -1;
		int hpow = -1;
		int tw = 0;
		int th = 0;

		if (wpow > 0) {
			tw = tex24.width << wpow;
		} else if (wpow < 0) {
			tw = tex24.width >> -wpow;
		}
		if (hpow > 0) {
			th = tex24.height << hpow;
		} else if (hpow < 0) {
			th = tex24.height >> -hpow;
		}
		byte[] data = tex24.GetRawTextureData ();
		byte[] result = new byte[th * tw * 3];


		TimeSpan ts;
		DateTime dt = DateTime.Now;
		BilinearFilter.Scale24 (data, tex32.width, tex32.height, wpow, hpow, result);
		ts = DateTime.Now - dt;
		print ("scale: " + ts);

		Texture2D t24 = new Texture2D(tw, th, TextureFormat.RGB24, false, true);
		t24.LoadRawTextureData (result);
		File.WriteAllBytes(Path.Combine(Application.dataPath, "test_scale24.png"), t24.EncodeToPNG());
	}

	public void Test32() {

		int wpow = -1;
		int hpow = -1;
		int tw = 0;
		int th = 0;

		if (wpow > 0) {
			tw = tex32.width << wpow;
		} else if (wpow < 0) {
			tw = tex32.width >> -wpow;
		}
		if (hpow > 0) {
			th = tex32.height << hpow;
		} else if (hpow < 0) {
			th = tex32.height >> -hpow;
		}
		byte[] data = tex32.GetRawTextureData ();
		byte[] result = new byte[th * tw * 4];


		TimeSpan ts;
		DateTime dt = DateTime.Now;
		BilinearFilter.Scale32 (data, tex32.width, tex32.height, wpow, hpow, result);
		ts = DateTime.Now - dt;
		print ("scale: " + ts);

		Texture2D t32 = new Texture2D(tw, th, TextureFormat.RGBA32, false, true);
		t32.LoadRawTextureData (result);
		File.WriteAllBytes(Path.Combine(Application.dataPath, "test_scale32.png"), t32.EncodeToPNG());
	}
}
