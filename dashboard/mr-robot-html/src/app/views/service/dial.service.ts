import { Injectable } from '@angular/core';
import {ContactItem} from "../phone-view/contacts-list/contact-item/contact-item";

@Injectable()
export class DialService {
  private dialNumber : string;
  private ongoingCall : boolean;
  private shouldSelect : boolean;
  private contact : ContactItem;

  constructor() {
    this.dialNumber = "";
    this.shouldSelect = false;
    this.contact = null;
  }

  public get getOngoingCall() {
    return this.ongoingCall;
  }

  public startCall() {
    this.ongoingCall = true;
  }

  public stopCall() {
    this.ongoingCall = false;
    this.setDialNumber("");
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
