import { Injectable } from '@angular/core';
import {Observable} from "rxjs";
import * as io from 'socket.io-client';
import {Http} from "@angular/http";

@Injectable()
export class ControlSocketService {

  private url : string = null;
  private port : string = null;
  private SERVER_FILE : string = "/assets/server.json";
  private socket;
  private onReady : Observable<void>;

  public static CONNECT_SUCCESS : number = 1;
  public static CONNECT_ERROR : number = 2;

  constructor( private http : Http) {

    // Load server config file
    let serverInfoObserver = this.http.get(this.SERVER_FILE).map((res : any) => res.json());

    // Create ready observable
    this.onReady = new Observable<void>((observer) => {

      // Subscribe to info
      serverInfoObserver.subscribe((data) => {
        this.url = data.url;
        this.port = data.port;
        observer.next();
      },(error) => {
        console.error("Server information not found: '" + this.SERVER_FILE +"' not found!");
      });
    });
  }

  private _formatMessage(code : number, object : any) {
    return {"code" : code, "object" : object};
  }

  public socketSubscribe(callback : (data : any) => any) {
    return this._getCarConnection().subscribe(callback);
  }

  public onReadySubscribe(callback : (data : any) => any) {
    return this.onReady.subscribe(callback);
  }

  private _getCarConnection() {

    // Create a new instance each time the component is loaded
    return new Observable((observer) => {
      this.socket = io(this.url +":"+ this.port);

      this.socket.on('message', (data) => {
        observer.next(this._formatMessage(ControlSocketService.CONNECT_SUCCESS, data));
      });

      this.socket.on('connect_error', (error) => {
        observer.next(this._formatMessage(ControlSocketService.CONNECT_ERROR, error));
      });

      this.socket.on('connect',() => {
        observer.next(this._formatMessage(ControlSocketService.CONNECT_SUCCESS, null));
      });

      return () => {
        this.socket.disconnect();
      }
    });
  }

  public getPort() : string {
    return this.port;
  }

  public getUrl() : string {
    return this.url;
  }

  public emitNavigation(direction : string, speedLeft : number, speedRight : number) {
    this.socket.emit('car-navigation', {"direction": direction,
      "speedLeft": speedLeft, "speedRight": speedRight});
  }

  public emitOCV(status : string, device : number, color : string) {
    this.socket.emit('car-ocv', {"status": status, "device" : device, "color": color});
  }
}
