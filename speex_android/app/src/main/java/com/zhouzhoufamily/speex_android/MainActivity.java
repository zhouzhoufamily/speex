package com.zhouzhoufamily.speex_android;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.speex.util.AudioFileUtils;
import com.speex.util.AudioRecordUtils;
import com.speex.util.PlayerUtils;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, View.OnTouchListener {

    Button mPlayBtn;

    Button mRecordBtn;

    PlayerUtils mPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        System.loadLibrary("speex");

        // Example of a call to a native method
        //TextView tv = (TextView) findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());

        initView();
    }

    void  initView(){
        mPlayBtn = (Button) findViewById(R.id.play_btn);
        mRecordBtn = (Button) findViewById(R.id.record_btn);
        mPlayBtn.setOnClickListener(this);
        mRecordBtn.setOnTouchListener(this);
        mPlayer = new PlayerUtils();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.play_btn:
                try{
                    mPlayer.setUrlPrepare(AudioFileUtils.getDecodeWavFilePath());
                    mPlayer.play();
                }catch (Exception e){

                }
                break;
        }
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        AudioRecordUtils utils = AudioRecordUtils.getInstance();
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                utils.startRecordAndFile();
                break;
            case  MotionEvent.ACTION_UP:
                utils.stopRecordAndFile();
                break;
        }
        return false;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();
}
