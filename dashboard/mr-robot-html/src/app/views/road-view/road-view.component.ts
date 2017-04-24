import {Component, OnInit, OnDestroy, ViewChild} from '@angular/core';
import {ControlSocketService} from "../service/control-socket.service";
import {Observable} from "rxjs";

@Component({
  selector: 'mr-road-view',
  templateUrl: './road-view.component.html',
  styleUrls: ['./road-view.component.scss']
})
export class RoadViewComponent implements OnInit, OnDestroy {
  private onReadyConnection;
  private socketConnection;
  private connected : boolean = false;

  // HTML elements
  @ViewChild("out") out;
  @ViewChild("bw") bw;
  @ViewChild("info") info;

  // Canvas elements
  private outImg;
  private bwImg;
  private periodTime = 300;
  private outImgCtx;
  private bwImgCtx;
  private imageTimer;
  private imageTimerSub;

  constructor(public controlSocketService : ControlSocketService) { }

  ngOnInit() {

    // Wait till all server info are fetched from configuration files
    this.onReadyConnection = this.controlSocketService.onReadySubscribe(() =>{

      // Establish socket connection
      this.socketConnection = this.controlSocketService.socketSubscribe((data : any) =>{
        if(data.code === ControlSocketService.CONNECT_ERROR) {
          this.connected = false;
        } else if(data.code === ControlSocketService.CONNECT_SUCCESS) {
          this.connected = true;
        }
      });

      // Init canvas
      this._initCanvas();

      // Update canvas
      this._updateImages();
    });

  }

  ngOnDestroy() {
    if(this.socketConnection != null) {
      this.socketConnection.unsubscribe();
    }

    if(this.onReadyConnection != null) {
      this.onReadyConnection.unsubscribe();
    }
    this.imageTimerSub.unsubscribe();
  }

  private _initCanvas() {

    // Main images
    this.outImg = new Image();
    this.bwImg = new Image();

    // Canvases
    this.outImgCtx = this.out.nativeElement.getContext("2d");
    this.bwImgCtx = this.bw.nativeElement.getContext("2d");

  }

  private _updateImages() {

    // Create timer
    this.imageTimer = Observable.timer(0,1000); // Every second starting from now
    this.imageTimerSub = this.imageTimer.subscribe(t => {

      // Load images
      this.outImg.src = this.controlSocketService.getUrl()+":"+this.controlSocketService.getPort()+"/debug/out.jpg?d=" + Date.now();
      this.bwImg.src = this.controlSocketService.getUrl()+":"+this.controlSocketService.getPort()+"/debug/bw.jpg?d=" + Date.now();

      // Draw images
      this.outImgCtx.drawImage(this.outImg, 0, 0);
      this.bwImgCtx.drawImage(this.bwImg, 0, 0);
    });
}

  onStartPress() {
    console.log("Start pressed!");
    this.controlSocketService.emitOCV("start",0, "blue");
  }

  onStopPress() {
    console.log("Stop pressed!");
    this.controlSocketService.emitOCV("stop", 0, "blue");
  }
}
