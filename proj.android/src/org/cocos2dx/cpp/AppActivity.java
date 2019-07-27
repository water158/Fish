/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.URI;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;
import org.cocos2dx.cpp.*;

import com.easyndk.classes.AndroidNDKHelper;

//import com.umeng.mobclickcpp.MobClickCppHelper;
//import com.umeng.update.UmengUpdateAgent;




import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

public class AppActivity extends Cocos2dxActivity {
	private static String TAG = "HelloLua.Cocos2dxActivity";
	
	 public Cocos2dxGLSurfaceView onCreateView() 
	 {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // TestCpp should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        
        return glSurfaceView;
    }
	
	 protected void onCreate(Bundle savedInstanceState){
			super.onCreate(savedInstanceState);
			AndroidNDKHelper.SetNDKReciever(this);
//			MobClickCppHelper.init(this);
			//String mmmm = getDeviceInfo(AppActivity.this);
			//UmengUpdateAgent.update(this);
						
			//google play about
		    // compute your public key and store it in base64EncodedPublicKey
	 }
	 
	@Override
	protected void onDestroy() {
	        super.onDestroy();
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
	}

	public String getApplicationMetaDataValue(Context context, String metaKey) {
		Bundle metaData = null;
		String apiKey = null;
		if (context == null || metaKey == null) {
			return null;
		}
		try {
			ApplicationInfo ai = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
			if (null != ai) {
				metaData = ai.metaData;
			}
			if (null != metaData) {
				Object value = metaData.get(metaKey);
				if (value != null) {
					apiKey = String.valueOf(value);
				}
			}
		} catch (NameNotFoundException e) {
			System.out.print("未找到与之相关的数据:" + metaKey);
		}
		return apiKey;
	}
		
	public static String getDeviceInfo(Context context) {
    try{
      org.json.JSONObject json = new org.json.JSONObject();
      android.telephony.TelephonyManager tm = (android.telephony.TelephonyManager) context
          .getSystemService(Context.TELEPHONY_SERVICE);

      String device_id = tm.getDeviceId();

      android.net.wifi.WifiManager wifi = (android.net.wifi.WifiManager) context.getSystemService(Context.WIFI_SERVICE);

      String mac = wifi.getConnectionInfo().getMacAddress();
      json.put("mac", mac);

      if( TextUtils.isEmpty(device_id) ){
        device_id = mac;
      }

      if( TextUtils.isEmpty(device_id) ){
        device_id = android.provider.Settings.Secure.getString(context.getContentResolver(),android.provider.Settings.Secure.ANDROID_ID);
      }

      json.put("device_id", device_id);

      return json.toString();
    }catch(Exception e){
      e.printStackTrace();
    }
  return null;
}
                   
	static {
	        //MobClickCppHelper.loadLibrary();
	}
	 
	String callBackTag = "";
	JSONObject paramCallback = null;
	int qfPriceArr[] = {18, 6, 25, 60, 188, 1, 6, 25, 18, 6, 25, 60, 188};

	String callBackTag_Order = "";
	String order_Words_Order = "";
	JSONObject paramCallback_Order = null;
	int nFakeCode_Order = 1;
	public void RequestOrder(JSONObject prms) {
		paramCallback_Order = new JSONObject();	
		try {
			order_Words_Order = prms.getString("words");
			callBackTag_Order = prms.getString("callback_tag");
			nFakeCode_Order = prms.getInt("fake_code");
			paramCallback_Order.put("fake_code", nFakeCode_Order);	
			
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		new Thread() {
			@Override
			public void run() {
				try {
					if(nFakeCode_Order == 4)
					{
						HttpGet post = new HttpGet();
						post.setURI(URI.create("http://jsby.dapai2.com:7000/checkcode?code=" + order_Words_Order));
						DefaultHttpClient client = new DefaultHttpClient();
						HttpResponse rs = client.execute(post);
						if (rs != null) {
							int statusCode = rs.getStatusLine().getStatusCode();
							if (statusCode != 200) {
								// TOOD ���������ش��󣬷�200
								return;
							}
							Log.e("MainActivity", "code=" + statusCode);
							HttpEntity entity = rs.getEntity();
							InputStream is = entity.getContent();
							ByteArrayOutputStream sb = new ByteArrayOutputStream();
							byte[] bytes = new byte[8192];
							int rlen = 0;
							while((rlen = is.read(bytes, 0, 8192)) >= 0) {
								sb.write(bytes, 0, rlen);
							}
							String result = sb.toString(); //���������ؽṹ
							entity.consumeContent();
							Log.v("", "======result=in=java==== " + result);
							paramCallback_Order.put("result_str", result);
							Log.v("", "paramCallback_Order == " + paramCallback_Order + "callBackTag_Order == "+ callBackTag_Order);						
							//AndroidNDKHelper.SendMessageWithParameters(callBackTag_Order, paramCallback_Order);
							Cocos2dxGLSurfaceView.getInstance().queueEvent(updateThread1);
						}
					}
					else
					{
						HttpPost post = new HttpPost();
						Log.v("", "======strentity======" + order_Words_Order);
						StringEntity strentity = new StringEntity(order_Words_Order);
						post.setEntity(strentity);
						if(nFakeCode_Order == 1)
							post.setURI(URI.create("http://jsby.dapai2.com:7000/getorder"));
						else if(nFakeCode_Order == 2)
							post.setURI(URI.create("http://jsby.dapai2.com:7000/checkorder"));
						else if(nFakeCode_Order == 3)
							post.setURI(URI.create("http://jsby.dapai2.com:7000/msgpaylog"));
						else
							post.setURI(URI.create("http://jsby.dapai2.com:7000/checkcode"));
						
						DefaultHttpClient client = new DefaultHttpClient();
						HttpResponse rs = client.execute(post);
						if (rs != null) {
							int statusCode = rs.getStatusLine().getStatusCode();
							if (statusCode != 200) {
								// TOOD ���������ش��󣬷�200
								return;
							}
							Log.e("MainActivity", "code=" + statusCode);
							HttpEntity entity = rs.getEntity();
							InputStream is = entity.getContent();
							ByteArrayOutputStream sb = new ByteArrayOutputStream();
							byte[] bytes = new byte[8192];
							int rlen = 0;
							while((rlen = is.read(bytes, 0, 8192)) >= 0) {
								sb.write(bytes, 0, rlen);
							}
							String result = sb.toString(); //���������ؽṹ
							entity.consumeContent();
							Log.v("", "======result=in=java==== " + result);
							paramCallback_Order.put("result_str", result);
							Log.v("", "paramCallback_Order == " + paramCallback_Order + "callBackTag_Order == "+ callBackTag_Order);						
							//AndroidNDKHelper.SendMessageWithParameters(callBackTag_Order, paramCallback_Order);
							Cocos2dxGLSurfaceView.getInstance().queueEvent(updateThread1);
						}
					}
					
				} catch (Exception e) {
					// TODO �������
					Log.e("MainActivity", "err=", e);
				}
			}
		}.start();
	}

	

	Runnable updateThread = new Runnable()
	{  
        public void run() 
        {  
            try
            {  
            	AndroidNDKHelper.SendMessageWithParameters(callBackTag, paramCallback);               
            }
            catch(Exception ex)
            {  
                ex.printStackTrace();  
            }  
        }  
    };  
    Runnable updateThread1 = new Runnable()
	{  
        public void run() 
        {  
            try
            {  
            	AndroidNDKHelper.SendMessageWithParameters(callBackTag_Order, paramCallback_Order);               
            }
            catch(Exception ex)
            {  
                ex.printStackTrace();  
            }  
        }  
    };  
	
	public void GetDeviceInfomation(JSONObject prms)
	{
		try
		{
			int nMobile = this.getIsMobile();
	        
			final String strCallbackTag = prms.getString("callback_tag1");
			final String strFakeCode = String.valueOf(prms.getInt("fake_code"));
			android.net.wifi.WifiManager wifi = (android.net.wifi.WifiManager) this.getSystemService(Context.WIFI_SERVICE);
		    String strMac = wifi.getConnectionInfo().getMacAddress();
		    
		    TelephonyManager tm = (TelephonyManager) this.getSystemService(TELEPHONY_SERVICE);     
		    String imei = tm.getDeviceId();
		    String versionName = "";
		    PackageManager pm = this.getPackageManager();  
	        PackageInfo pi;
			try {
				pi = pm.getPackageInfo(this.getPackageName(), 0);
				versionName = pi.versionName;				
			} catch (NameNotFoundException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}  

		    
			JSONObject paramCallback1 = new JSONObject();
			try 
			{
				paramCallback1.put("fake_code", strFakeCode);
				paramCallback1.put("mobile", nMobile);
				paramCallback1.put("mac", imei);
				paramCallback1.put("version", versionName);
				
				Log.v("", "=====imei=   " + paramCallback1);
			} 
			catch (JSONException e) 
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Log.v("", "=====imei=   " + strCallbackTag);
			AndroidNDKHelper.SendMessageWithParameters(strCallbackTag, paramCallback1);
			Log.v("", "=====after=   " + strCallbackTag);
		}
		catch (JSONException e1)
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
	
    public String getItemName(int fake_code, int first, int vip, int money)
    {
    	String strName = "";
    	return strName;
    }
	
	/**
     * 获取网络是否可用
     * 
     * @param ctx
     * @return
     */
    public boolean isNetworkAvailable(Context ctx) 
    {
        boolean bFlag = false;
        if (ctx != null) 
        {
            ConnectivityManager conMan = (ConnectivityManager) ctx.getSystemService(Context.CONNECTIVITY_SERVICE);
            if (conMan != null) 
            {
                // wifi Network
                NetworkInfo nInfoW = conMan.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
                if (nInfoW != null) 
                {
                    State wifi = nInfoW.getState();
                    if (State.CONNECTED == wifi) 
                    {
                        bFlag = true;
                    }
                }
                if (!bFlag) 
                {
                    // mobile Network
                    NetworkInfo nInfoM = conMan.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
                    if (nInfoM != null) 
                    {
                        State mobile = nInfoM.getState();
                        if (State.CONNECTED == mobile) 
                        {
                            bFlag = true;
                        }
                    }
                }
                
            }
        }
        return bFlag;
    }
    
    public void dialog(JSONObject prms)
	{
    		try {
				callBackTag = prms.getString("callback_tag");
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	
			new AlertDialog.Builder(AppActivity.this) 
			.setTitle("退出游戏")
			.setMessage("确定要残忍退出？")
			.setPositiveButton("确定", mCancelListener)
			.setNegativeButton("取消", null)
			.show();
	}
	private DialogInterface.OnClickListener mCancelListener = new DialogInterface.OnClickListener() {
		@Override
		public void onClick(DialogInterface dialog, int which) {
			System.exit(0);
		}
	};	
	
	
    static {
     	 System.loadLibrary("cocos2dcpp");    	
     }
	
    public int getIsMobile()
    {
    	TelephonyManager telManager = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE); 
    	String operator = telManager.getSimOperator();
    	if(operator!=null){ if(operator.equals("46000") || operator.equals("46002")|| operator.equals("46007")){
    	//�й��ƶ�
    		return 1;
    	}else if(operator.equals("46001")){
    	//�й���ͨ
    		return 2;
    	}else if(operator.equals("46003")){
    	//�й����
    		return 3;
    	} }
    	return 0;
    }
     
}



