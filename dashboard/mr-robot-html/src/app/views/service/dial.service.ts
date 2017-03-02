import { Injectable } from '@angular/core';
import {ContactItem} from "../phone-view/contacts-list/contact-item/contact-item";
import Timer = NodeJS.Timer;

@Injectable()
export class DialService {
  private dialNumber : string;
  private ongoingCall : boolean;
  private shouldSelect : boolean;
  private contact : ContactItem;

  private seconds : number;
  private minutes : number;
  private callTime : string;
  private callInterval : Timer;

  constructor() {
    this.dialNumber = "";
    this.shouldSelect = false;
    this.ongoingCall = false;
    this.contact = null;
    this.callInterval = null;
  }

  public get getOngoingCall() {
    return this.ongoingCall;
  }

  public startCall() {
    this.ongoingCall = true;
    this.callTime = "Calling ...";
    this.seconds = 0;
    this.minutes = 0;

    // Start timer
    this.callInterval = setInterval(()=>{
      this.callTime = this.minutes+":"+(this.seconds < 10 ? "0"+this.seconds : this.seconds);
      if(this.seconds == 59){
        this.minutes++;
      }
      this.seconds = (this.seconds+1)%60;
    },1000);
  }

  public get getCallTime() {
    return this.callTime;
  }

  public stopCall() {
    this.ongoingCall = false;
    this.setDialNumber("");
    if(this.callInterval != null) {
      clearInterval(this.callInterval);
      this.callInterval = null;
    }
  }

  public enableShouldSelectNumber(contact : ContactItem) {
    this.shouldSelect = true;
    this.contact = contact;
  }

  public disableShouldSelectNumber() {
    this.shouldSelect = false;
    this.contact = null;
  }

  public get shouldSelectNumber() {
    return this.shouldSelect;
  }

  public get getContactOnSelectNumber() {
    return this.contact;
  }

  public setDialNumber(val : string) {
      this.dialNumber = val;
  }

  public get getDialNumber() {
    return this.dialNumber;
  }
}
