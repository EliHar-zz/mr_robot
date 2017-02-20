import { Injectable } from '@angular/core';
import {AppItemClass} from "../apps-view/app-item/app-item";

@Injectable()
export class ViewDataService {
  private views : AppItemClass[];

  constructor() {
    this.views = [];
  }

  public getViews() {
    return this.views;
  }

  public addView(view : AppItemClass) {
    // Verify that they have unique name
    for(let newView of this.views) {
      if(view.title === newView.title) {
        console.error("View not added: '" + view.title + "' already exists!");
        return;
      }
    }

    // Add view
    this.views.push(view);
  }
}
