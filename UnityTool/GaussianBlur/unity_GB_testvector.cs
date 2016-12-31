

using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

using System.Collections;
using System.IO;
using System;

using Mard.Tools.Blur;

[ExecuteInEditMode]
public class unity_GB_testvector : MonoBehaviour {

	public Image img32;
	public Image img24;
	public Sprite sp32;
	public Sprite sp24;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnEnable() {
		if ((null == img32 && null == img24) || (null == sp32 && null == sp24)) {
			print ("<color=red> not available image or component");
			return;
		}

		if (null != sp32)
			Test32 ();
		if (null != sp24)
			Test24 ();

		AssetDatabase.Refresh ();
	}

	public void Test24 ()
	{
		Texture2D tex24 = sp24.texture;
		byte[] data = tex24.GetRawTextureData ();
		print (data.Length);
		print ("width: " + tex24.width);
		print ("height: " + tex24.height);

		byte[] result_24 = new byte[data.Length];
		byte[] result_24h = new byte[data.Length];
		byte[] result_24_4 = new byte[data.Length];
		byte[] result_24v = new byte[data.Length];

		TimeSpan ts;
		DateTime dt = DateTime.Now;
		//GaussianBlur.Blur24 (data, tex24.width, tex24.height, 1, 5, result_24_4);
		ts = DateTime.Now - dt;
		print ("blur24_4: " + ts);
		dt = DateTime.Now;
		//GaussianBlur.Blur24 (data, tex24.width, tex24.height, result_24);
		ts = DateTime.Now - dt;
		print ("blur24: " + ts);
		dt = DateTime.Now;
		for (int i = 0; i < GaussianBlur.cDefaultRadius + GaussianBlur.cDefaultRadius + 1; i++)
			GaussianBlur.Blur24Horizontal (data, tex24.width, tex24.height, i, result_24h);
		ts = DateTime.Now - dt;
		print ("blur24h: " + ts);
		dt = DateTime.Now;
		for (int i = 0; i < GaussianBlur.cDefaultRadius + GaussianBlur.cDefaultRadius + 1; i++)
			GaussianBlur.Blur24Vertical (data, tex24.width, tex24.height, i, result_24v);
		ts = DateTime.Now - dt;
		print ("blur24v: " + ts);

		//Texture2D t24 = new Texture2D(tex24.width, tex24.height, TextureFormat.RGB24, false, true);
		//t24.LoadRawTextureData (result_24);
		//File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur24.png"), t24.EncodeToPNG());

		Texture2D t24h = new Texture2D(tex24.width, tex24.height, TextureFormat.RGB24, false, true);
		t24h.LoadRawTextureData (result_24h);
		File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur24h.png"), t24h.EncodeToPNG());

		Texture2D t24v = new Texture2D(tex24.width, tex24.height, TextureFormat.RGB24, false, true);
		t24v.LoadRawTextureData (result_24v);
		File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur24v.png"), t24v.EncodeToPNG());

		//Texture2D t24_4 = new Texture2D(tex24.width, tex24.height, TextureFormat.RGB24, false, true);
		//t24_4.LoadRawTextureData (result_24_4);
		//File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur24_4.png"), t24_4.EncodeToPNG());
	}

	public void Test32()
	{
		Texture2D tex32 = sp32.texture;
		byte[] data = tex32.GetRawTextureData ();
		print (data.Length);
		print ("width: " + tex32.width);
		print ("height: " + tex32.height);

		byte[] result_32 = new byte[data.Length];
		byte[] result_32_4 = new byte[data.Length];
		byte[] result_32h = new byte[data.Length];
		byte[] result_32v = new byte[data.Length];

		byte[] result_32_repeat = new byte[data.Length];
		byte[] result_32_src = null;
		byte[] result_32_dst = null;
		byte[] result_32_tmp = null;

		TimeSpan ts;
		DateTime dt = DateTime.Now;
		//GaussianBlur.Blur32 (data, tex32.width, tex32.height, 2, 4, result_32_4);
		ts = DateTime.Now - dt;
		print ("blur32_4: " + ts);
		dt = DateTime.Now;
		GaussianBlur.Blur32 (data, tex32.width, tex32.height, 100.5f, 3, result_32);
		ts = DateTime.Now - dt;
		print ("blur32: " + ts);
		dt = DateTime.Now;
		//GaussianBlur.Blur32 (data, tex32.width, tex32.height, result_32);
		//for (int i = 0; i < GaussianBlur.cDefaultRadius + GaussianBlur.cDefaultRadius + 1; i++)
		//	GaussianBlur.Blur32Horizontal (data, tex32.width, tex32.height, i, result_32h);
		ts = DateTime.Now - dt;
		print ("blur32h: " + ts);
		dt = DateTime.Now;
		//for (int i = 0; i < GaussianBlur.cDefaultRadius + GaussianBlur.cDefaultRadius + 1; i++)
		//	GaussianBlur.Blur32Vertical (data, tex32.width, tex32.height, i, result_32v);
		ts = DateTime.Now - dt;
		print ("blur32v: " + ts);
		dt = DateTime.Now;
		result_32_src = data;
		result_32_dst = result_32_repeat;
		float sd = 5.5f;
		int r = 3;
		float[] matrix = GaussianMatrixGen.GetGaussianMatrixIn2d (sd, r);
		for (int i = 0; i < r + r + 1; i++) {
			GaussianBlur.Blur32Horizontal (result_32_src, tex32.width, tex32.height, matrix, sd, r, i, result_32_dst);
		}
		result_32_src = result_32_repeat;
		result_32_dst = data;
		for (int i = 0; i < r + r + 1; i++) {
			GaussianBlur.Blur32Vertical (result_32_src, tex32.width, tex32.height, matrix, sd, r, i, result_32_dst);
		}
		result_32_src = data;
		result_32_dst = result_32_repeat;
		GaussianBlur.Blur32 (result_32_src, tex32.width, tex32.height, 100.5f, 3, result_32_dst);
		ts = DateTime.Now - dt;
		print ("blur32-hv: " + ts);
		dt = DateTime.Now;
		sd = 0.84089642f;
		r = 3;
		ts = DateTime.Now - dt;
		print ("blur32-repeat: " + ts);

		Texture2D t32 = new Texture2D(tex32.width, tex32.height, TextureFormat.RGBA32, false, true);
		t32.LoadRawTextureData (result_32);
		File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur32.png"), t32.EncodeToPNG());

		//Texture2D t32_4 = new Texture2D(tex32.width, tex32.height, TextureFormat.RGBA32, false, true);
		//t32_4.LoadRawTextureData (result_32_4);
		//File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur32_4.png"), t32_4.EncodeToPNG());

		//Texture2D t32h = new Texture2D(tex32.width, tex32.height, TextureFormat.RGBA32, false, true);
		//t32h.LoadRawTextureData (result_32h);
		//File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur32h.png"), t32h.EncodeToPNG());

		//Texture2D t32v = new Texture2D(tex32.width, tex32.height, TextureFormat.RGBA32, false, true);
		//t32v.LoadRawTextureData (result_32v);
		//File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur32v.png"), t32v.EncodeToPNG());

		Texture2D t32repeat = new Texture2D(tex32.width, tex32.height, TextureFormat.RGBA32, false, true);
		t32repeat.LoadRawTextureData (result_32_dst);
		File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur32hv.png"), t32repeat.EncodeToPNG());
	}
}
