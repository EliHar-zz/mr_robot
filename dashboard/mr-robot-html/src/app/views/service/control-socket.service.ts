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

  public static CONNECT_SUCCESS : number = 1;
  public static CONNECT_ERROR : number = 2;

  constructor( private http : Http) {
    let test= this.http.get(this.SERVER_FILE).map((res : any) => res.json());
    test.subscribe((data) => {
      this.url = data.url;
      this.port = data.port;
    },(error) => {
      console.error("Server information not found: '" + this.SERVER_FILE +"' not found!");
    });
  }

  private _formatMessage(code : number, object : any) {
    return {"code" : code, "object" : object};
  }

  public getControlConnection() {
    let observable = new Observable((observer) => {
      this.socket = io(this.url +":"+ this.port);

      this.socket.on('message', (data) => {
        observer.next(this._formatMessage(ControlSocketService.CONNECT_SUCCESS, data));
      });

      this.socket.on('connect_error', (error) => {
        observer.next(this._formatMessage(ControlSocketService.CONNECT_ERROR, error));
      });

      return () => {
        this.socket.disconnect();
      }
    });
    return observable;
  }

  sendControl(value : string) {
    this.socket.emit('control-car', value)
  }
}
