package com.example.einere.myapplication.capture;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.Image;
import android.net.ConnectivityManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.RemoteException;
import android.support.annotation.Nullable;
import android.support.v4.app.FragmentActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.example.einere.myapplication.CameraActivity;
import com.example.einere.myapplication.GpsInfo;
import com.example.einere.myapplication.ListViewActivity;
import com.example.einere.myapplication.R;
import com.example.einere.myapplication.SocketManager;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.CameraPosition;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.PolylineOptions;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Locale;

public class CaptureActivity extends FragmentActivity implements SensorEventListener, OnMapReadyCallback, GoogleMap.OnMarkerClickListener {
    final int STATUS_DISCONNECTED = 0;
    final int STATUS_CONNECTED = 1;
    private final int CAMERA_CODE = 1111;
    private final int GALLERY_CODE = 1112;
    private final String TAG = "CaptureActivity";
    private GoogleMap mMap;

    // socket transmission
    SocketManager socketManager = null;

    // RecyclerView
    RecyclerView rv_selectedImage = null;
    RecyclerViewAdapter recyclerAdapter = null;
    Bitmap tmpBitmap = null;

    // sensor
    SensorManager sensorManager = null;
    Sensor accelerometer = null;
    Sensor magnetometer = null;
    float[] gravity = null;
    float[] geomagnetic = null;
    TextView tv_azimuth = null;
    private GpsInfo gps;

    // capture broadcast
    BroadcastReceiver cameraBroadcastReceiver = null;

    // upload ArrayList
    ArrayList<File> upImageList = new ArrayList<>();
    ArrayList<File> upTextList = new ArrayList<>();

    // id_number
    private String pointNum = null;
    private String workNum = "111";

    //image ArrayList
    private ArrayList<Image> images = new ArrayList<>();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_capture);

        // google map
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.Googlemap);
        mapFragment.getMapAsync(this); //getMapAsync must be called on the main thread.

        // get views
        rv_selectedImage = findViewById(R.id.rv_selectedImage);
        tv_azimuth = findViewById(R.id.tv_azimuth);

        // get sensor manager
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        magnetometer = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);

        // register camera capture broadcast receiver
        IntentFilter intentFilter = new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION);
        intentFilter.addAction("android.hardware.action.NEW_PICTURE");
        cameraBroadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                Log.i(TAG, String.format("azimuth1 : %s...", tv_azimuth.getText()));
                Toast.makeText(getBaseContext(), "captured!!!", Toast.LENGTH_SHORT).show();
            }
        };
        registerReceiver(cameraBroadcastReceiver, intentFilter);

        // make RecyclerView
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);
        linearLayoutManager.setOrientation(LinearLayoutManager.HORIZONTAL);
        rv_selectedImage.setLayoutManager(linearLayoutManager);
        recyclerAdapter = new RecyclerViewAdapter();
        rv_selectedImage.setAdapter(recyclerAdapter);

        // set listener
        findViewById(R.id.btn_capture).setOnClickListener(v -> capture());
        findViewById(R.id.btn_select_picture).setOnClickListener(v -> selectGallery());
        findViewById(R.id.btn_send_data).setOnClickListener(v -> sendData());
        findViewById(R.id.btn_test).setOnClickListener(v -> checkArrayListForUpload());
    }

    @Override
    protected void onResume() {
        super.onResume();

        // get SocketManager instance
        socketManager = SocketManager.getInstance();

        // register listener
        sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_UI);
        sensorManager.registerListener(this, magnetometer, SensorManager.SENSOR_DELAY_UI);
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // unregister broadcast receiver
        unregisterReceiver(cameraBroadcastReceiver);
    }


    /* ******************* google map start ******************* */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        // 구글 맵 객체를 불러온다.
        mMap = googleMap;

        // 서울 여의도에 대한 위치 설정
        LatLng seoul = new LatLng(37.52487, 126.92723);
        LatLng seoul2 = new LatLng(37.52440, 126.92750);

        // 구글 맵에 표시할 마커에 대한 옵션 설정
        MarkerOptions makerOptions = new MarkerOptions();
        makerOptions
                .position(seoul)
                .icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_YELLOW))
                .title("212");

        MarkerOptions makerOptions2 = new MarkerOptions();
        makerOptions2
                .position(seoul2)
                .title("213");
        // 마커를 생성한다.
        mMap.addMarker(makerOptions);
        mMap.addMarker(makerOptions2);
        mMap.setOnMarkerClickListener(this);

        PolylineOptions poly = new PolylineOptions();
        poly.color(Color.RED);
        poly.width(4);
        poly.add(seoul);
        poly.add(seoul2);
        mMap.addPolyline(poly);
        // 카메라 옵션
        CameraPosition cameraPosition = new CameraPosition.Builder()
                .target(seoul)      // Sets the center of the map to Mountain View
                .zoom(18)                   // Sets the zoom
                .bearing(0)                // Sets the orientation of the camera to east
                .tilt(30)                   // Sets the tilt of the camera to 30 degrees
                .build();
        // 카메라를 여의도 위치로 옮긴다.
        mMap.moveCamera(CameraUpdateFactory.newCameraPosition(cameraPosition));

    }

    public void refreshClick(View v) {
        mMap.clear();
    }

    @Override
    public boolean onMarkerClick(Marker marker) {
        ArrayList<Uri> uriList = new ArrayList<>();
        pointNum = marker.getTitle();

        File file = new File(Environment.getExternalStorageDirectory() + "/" + workNum);
        if (!file.exists()) {
            file.mkdir();
        }
        File file2 = new File(Environment.getExternalStorageDirectory() + "/" + workNum + "/" + pointNum);
        if (!file2.exists()) {
            file2.mkdir();
        }
        File uploadFile = new File(Environment.getExternalStorageDirectory() + "/" + workNum + "/" + pointNum + "/uploadfile");
        if (!uploadFile.exists()) {
            uploadFile.mkdir();
        }
        File file4 = new File(Environment.getExternalStorageDirectory() + "/" + workNum + "/" + pointNum + "/textfile");
        if (!file4.exists()) {
            file4.mkdir();
        }
        // 사진파일명 리스트 뽑아오기
        File[] uploadImageList2 = uploadFile.listFiles((dir, name) -> {
            Boolean bOK = false;
            if (name.toLowerCase().endsWith(".png")) bOK = true;
            if (name.toLowerCase().endsWith(".9.png")) bOK = true;
            if (name.toLowerCase().endsWith(".gif")) bOK = true;
            if (name.toLowerCase().endsWith(".jpg")) bOK = true;
            return bOK;
        });

        if (uploadImageList2 == null) {
            return true;
        }

        // 이미지와 텍스트 파일 list에 넣기
        for (File uploadImageFile : uploadImageList2) {
            upImageList.add(uploadImageFile);
            uriList.add(Uri.parse(uploadImageFile.getAbsolutePath()));

            // get path and fileName
            String path = uploadImageFile.getAbsolutePath();
            String textFileName = uploadImageFile.getAbsolutePath().substring(0, path.indexOf(".")) + ".txt";
            Log.d(TAG, String.format("textFileName : %s", textFileName));

            File textFile = new File(textFileName);
            if (textFile.exists()) {
                upTextList.add(textFile);
                Log.d(TAG, String.format("exist textFileName : %s", textFile.getAbsolutePath()));
            }
        }

        // clear RecyclerView
        recyclerAdapter.clearUriList();
        if (uriList.size() > 0) {
            recyclerAdapter.addUriList(uriList);
        }

        return true;
    }
    /* ******************* google map end ******************* */


    /* ******************* socket methods start ******************* */
    public void clickPoint(View v) {
        Intent intent = new Intent(this, ListViewActivity.class);
        startActivity(intent);
    }

    public void sendData() {
        // check manager status
        try {
            if (socketManager.getStatus() == STATUS_CONNECTED) {
                // get uri list
//                ArrayList<Uri> uriList = recyclerAdapter.getSelectedUriList();

                // make json object
                JSONObject packet = new JSONObject();
                JSONObject data = new JSONObject();
                packet.put("method", "GET");
                packet.put("subject", "test");
                data.put("userName", socketManager.getUserName());

                // put bitmap data
                int i = 0;
                ByteArrayOutputStream stream = new ByteArrayOutputStream();
                /*for (Uri uri : uriList) {
                    tmpBitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), uri);
                    tmpBitmap.compress(Bitmap.CompressFormat.JPEG, 100, stream);
                    byte[] bytes = stream.toByteArray();
                    String serialized = Base64.encodeToString(bytes, Base64.NO_WRAP);
                    data.put(String.format(Locale.KOREA, "image%d", i), serialized);
                    i++;
                }*/
                for (File file : upImageList) {
                    Bitmap tmpBitmap = BitmapFactory.decodeFile(file.getAbsolutePath());
                    tmpBitmap.compress(Bitmap.CompressFormat.JPEG, 100, stream);
                    byte[] bytes = stream.toByteArray();
                    String serialized = Base64.encodeToString(bytes, Base64.NO_WRAP);
                    data.put(String.format(Locale.KOREA, "image%d", i), serialized);
                }
                // put text data
                String content = getFileContents(upTextList.get(0));

                packet.put("data", data);

                // send to server
                socketManager.send(packet.toString());
                Toast.makeText(this, "send!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this, "not connected to server or no selected image", Toast.LENGTH_SHORT).show();
            }
        } catch (RemoteException e) {
            Toast.makeText(this, "RemoteException occurred!", Toast.LENGTH_SHORT).show();
        } catch (JSONException e) {
            Toast.makeText(this, "JSONException occurred!", Toast.LENGTH_SHORT).show();
        } /*catch (FileNotFoundException e) {
            Toast.makeText(this, "FileNotFoundException occurred!", Toast.LENGTH_SHORT).show();
        } catch (IOException e) {
            Toast.makeText(this, "IOException occurred!", Toast.LENGTH_SHORT).show();
        }*/

    }

    public void receiveData(View v) throws RemoteException {
        if (socketManager.getStatus() == STATUS_CONNECTED) {
            socketManager.receive();
        } else {
            Toast.makeText(this, "not connected to server", Toast.LENGTH_SHORT).show();
        }
    }
    /* ******************* socket methods end ******************* */


    /* ******************* sensor methods start ******************* */
    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            gravity = event.values;
        }
        if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
            geomagnetic = event.values;
        }

        if (gravity != null && geomagnetic != null) {
            float R[] = new float[9];
            float I[] = new float[9];

            boolean success = SensorManager.getRotationMatrix(R, I, gravity, geomagnetic);
            if (success) {
                float orientation[] = new float[3];
                SensorManager.getOrientation(R, orientation);

                // orientation contains: azimuth, pitch  and roll
                float azimuth = orientation[0];
                float pitch = orientation[1];
                float roll = orientation[2];

                tv_azimuth.setText(String.format(Locale.KOREA, "azimuth : %f", azimuth));
//                txtPitch.setText("y 좌표:" + String.valueOf(pitch));
//                txtRoll.setText("z 좌표 : " + String.valueOf(roll));
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }
    /* ******************* sensor methods end ******************* */


    /* ******************* GPS methods start ******************* */
    public void Gps() {
        gps = new GpsInfo(this);
        // GPS 사용유무 가져오기
        if (gps.isGetLocation()) {
            double latitude = gps.getLatitude();
            double longitude = gps.getLongitude();

            Toast.makeText(
                    getApplicationContext(),
                    "당신의 위치 - \n위도: " + latitude + "\n경도: " + longitude,
                    Toast.LENGTH_LONG).show();
        } else {
            // GPS 를 사용할수 없으므로
            gps.showSettingsAlert();
        }
    }
    /* ******************* GPS methods end ******************* */


    /* ******************* camera & gallery methods start ******************* */
    public void capture() {
        if (pointNum != null) {
            Intent intent = new Intent(this, CameraActivity.class);
            Bundle bundleData = new Bundle();
            bundleData.putString("c_point_num", pointNum);
            bundleData.putString("work_num", workNum);
            intent.putExtra("ID_NUM", bundleData);
            startActivity(intent);
        } else {
            Toast.makeText(getBaseContext(), "작업할 마커를 선택해주세요.", Toast.LENGTH_SHORT).show();
        }

    }

    public void selectGallery() {
        // call ImagePickerActivity
        if (pointNum != null) {
            Intent intent = new Intent(this, ImagePickerActivity.class);

            Bundle bundleData = new Bundle();
            bundleData.putString("c_point_num", pointNum);
            bundleData.putString("work_num", workNum);
            intent.putExtra("ID_NUM", bundleData);

            startActivityForResult(intent, GALLERY_CODE);
        } else {
            Toast.makeText(getBaseContext(), "작업할 마커를 선택해주세요.", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {
            switch (requestCode) {
                case GALLERY_CODE: {
                    // if use forth method
                    try {
                        ArrayList<Uri> uris = data.getParcelableArrayListExtra("selected");
                        for (Uri uri : uris) {
                            // add to RecyclerView
                            recyclerAdapter.addUri(uri);
                            Log.d(TAG, String.format("received uri : %s", uri.toString()));

                            // get image file path from uri
                            String path = uri.getPath();

                            // 선택한 이미지를 업로드용 폴더에 추가 (비정상 종료를 대비하기 위함)
                            upImageList.add(copyAndReturnFile(path, "IMAGE"));

                            // 선택한 이미지와 같은 이름의 텍스트 파일을 업로드용 폴더에 추가 (비정상 종료를 대비하기 위함)
                            upTextList.add(copyAndReturnFile(path, "TEXT"));
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Toast.makeText(this, "failed to receive image data...", Toast.LENGTH_SHORT).show();
                    }

                    break;
                }
                case CAMERA_CODE: {
                    //카메라에서 가져오기
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    public File copyAndReturnFile(String path, String mode) {
        // get file name, path
        String fileName;
        String filePath;

        String[] splitPath = path.split("/");
        fileName = splitPath[splitPath.length - 1];
        if (mode.equals("TEXT")) {
            fileName = splitPath[splitPath.length - 1];
            fileName = fileName.substring(0, fileName.indexOf(".")) + ".txt";
            splitPath[splitPath.length - 1] = "textfile/";
        }
        filePath = TextUtils.join("/", splitPath);
        if (mode.equals("TEXT")) filePath += fileName;
        Log.d(TAG, String.format("path : %s, filePath : %s, fileName : %s", path, filePath, fileName));

        // get file
        File file = new File(filePath);
        if (file.exists()) {
            try {
                // make streams
                FileInputStream fis = new FileInputStream(file);
                FileOutputStream newFos = new FileOutputStream(String.format("%s/%s/%s/%s/%s", Environment.getExternalStorageDirectory(), workNum, pointNum, "uploadfile", fileName));

                // copy
                int readCount = 0;
                byte[] buffer = new byte[1024];
                while ((readCount = fis.read(buffer, 0, 1024)) != -1) {
                    newFos.write(buffer, 0, readCount);
                }

                // close streams
                newFos.close();
                fis.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
            return file;
        }
        return null;
    }
    /* ******************* camera & gallery methods end ******************* */


    /* ******************* text file methods start ******************* */
    public String getFileContents(final File file) {
        final StringBuilder stringBuilder = new StringBuilder();

        try {
            final InputStream inputStream = new FileInputStream(file);
            final BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
            boolean done = false;

            while (!done) {
                final String line = reader.readLine();
                done = (line == null);

                if (line != null) {
                    stringBuilder.append(line);
                }
            }

            reader.close();
            inputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return stringBuilder.toString();
    }
    /* ******************* text file methods end ******************* */

    /* ******************* ArrayList for upload test methods start ******************* */
    public void checkArrayListForUpload() {
        if (upImageList != null) {
            for (File file : upImageList) {
                Log.d(TAG, String.format("image : %s", file.getAbsolutePath()));
            }
        }

        char[] buf = new char[1024];
        int size = 0;
        if (upTextList != null) {
            for (File file : upTextList) {
                Log.d(TAG, file.getPath());
                try {
                    FileReader fr = new FileReader(file);
                    BufferedReader br = new BufferedReader(fr);
                    Log.d(TAG, String.format("text : %s", br.readLine()));
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    /* ******************* ArrayList for upload test methods end ******************* */
}
