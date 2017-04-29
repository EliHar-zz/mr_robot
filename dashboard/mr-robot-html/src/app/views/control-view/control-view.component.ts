import {Component, OnInit, OnDestroy} from '@angular/core';
import {ControlSocketService} from "../service/control-socket.service";

@Component({
  selector: 'mr-control-view',
  templateUrl: './control-view.component.html',
  styleUrls: ['./control-view.component.scss']
})
export class ControlViewComponent implements OnInit, OnDestroy {

  private onReadyConnection;
  private socketConnection;
  private connected : boolean = false;

  constructor(private controlSocketService : ControlSocketService) { }

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
    });
  }

  ngOnDestroy() {
    if(this.socketConnection != null) {
      this.socketConnection.unsubscribe();
    }

    if(this.onReadyConnection != null) {
      this.onReadyConnection.unsubscribe();
    }
  }

  onUpPress() {
    console.log("Up pressed!");
    this.controlSocketService.emitNavigation("up", 255, 255);
  }

  onDownPress() {
    console.log("Down pressed!");
    this.controlSocketService.emitNavigation("down", -255, -255);
  }

  onLeftPress() {
    console.log("Left pressed!");
    this.controlSocketService.emitNavigation("left", -255, 255);
  }

  onRightPress() {
    console.log("Right pressed!");
    this.controlSocketService.emitNavigation("right", 255, -255);
  }

  onStopPress() {
    console.log("Stop pressed!");
    this.controlSocketService.emitNavigation("stop", 0, 0);
  }

  onUpRightPress() {
    console.log("Up Right pressed!");
    this.controlSocketService.emitNavigation("up-right", 255, 25);
  }

  onUpLeftPress() {
    console.log("Up Left pressed!");
    this.controlSocketService.emitNavigation("up-left", 0, 255);
  }

  onDownRightPress() {
    console.log("Down Right pressed!");
    this.controlSocketService.emitNavigation("down-right", -255, -25);
  }

  onDownLeftPress() {
    console.log("Down Left pressed!");
    this.controlSocketService.emitNavigation("down-left", 0, -255);
  }
}
