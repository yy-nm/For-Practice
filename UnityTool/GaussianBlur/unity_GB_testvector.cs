

using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

using System.Collections;
using System.IO;

using Mard.Tools.Blur;

[ExecuteInEditMode]
public class unity_GB_testvector : MonoBehaviour {

	public RawImage targetcomponent;
	public Texture2D tex;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnEnable() {
		if (null == targetcomponent || null == tex) {
			print ("<color=red> not available image or component");
			return;
		}

		byte[] data = tex.GetRawTextureData ();
		print (data.Length);
		print ("width: " + tex.width);
		print ("height: " + tex.height);

		int[] d = new int[tex.width * tex.height];

		for (int i = 0; i < tex.height; i++) {
			for (int j = 0; j < tex.width; j++) {
				d[i * tex.width + j] = data [i * tex.width * 4 + j * 4] << 24
				| data [i * tex.width * 4 + j * 4 + 1] << 16
				| data [i * tex.width * 4 + j * 4 + 2] << 8
				| data [i * tex.width * 4 + j * 4 + 3];
			}
		}

		int[] result = new int[d.Length];
		byte[] result_b = new byte[data.Length];

		GaussianBlur.Blur (d, tex.width, tex.height, result);

		for (int i = 0; i < tex.height; i++) {
			for (int j = 0; j < tex.width; j++) {
				result_b [i * tex.width * 4 + j * 4] = (byte)(result [i * tex.width + j] >> 24);
				result_b [i * tex.width * 4 + j * 4 + 1] = (byte)(result [i * tex.width + j] >> 16);
				result_b [i * tex.width * 4 + j * 4 + 2] = (byte)(result [i * tex.width + j] >> 8);
				result_b [i * tex.width * 4 + j * 4 + 3] = (byte)(result [i * tex.width + j]);
			}
		}

		Texture2D t = new Texture2D(tex.width, tex.height, TextureFormat.RGBA32, false, true);
		t.LoadRawTextureData (result_b);

		targetcomponent.texture = t;

		File.WriteAllBytes(Path.Combine(Application.dataPath, "test_gaussianblur.png"), t.EncodeToPNG());

		AssetDatabase.Refresh ();
	}

	public static void Test()
	{
		
	}
}
